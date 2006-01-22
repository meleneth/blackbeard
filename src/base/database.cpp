#include "database.hpp"
#include "config.hpp"
#include "console.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void save_db_data()
{
    string filename = config->blackbeard_data_dir + "/blackbeard.db";
    int rc;
    sqlite3* db;

    unlink(filename.c_str());
    rc = sqlite3_open(filename.c_str(), &db);
    console->log("Database file: " + filename);
    if(rc != SQLITE_OK){
        console->log("Could not create database " + filename);
    }
    setup_newsgroup_tables(db);
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    save_subscribed_groups_to_db(db);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    sqlite3_close(db);
}

void restore_db_data()
{
    string filename = config->blackbeard_data_dir + "/blackbeard.db";
    int rc;
    sqlite3* db;

    rc = sqlite3_open(filename.c_str(), &db);
    console->log("Database file: " + filename);
    if(rc != SQLITE_OK){
        console->log("Could not open database " + filename);
    }
    restore_newsgroups_from_db(db);
    sqlite3_close(db);

}

void restore_newsgroups_from_db(sqlite3 *db)
{
    sqlite3_stmt *s;
    string stmt = "SELECT newsgroup_no, name FROM newsgroups";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);
    while (SQLITE_ROW == sqlite3_step(s)){
        NewsGroup *group = group_for_name((char *)sqlite3_column_text(s, 1));
        group->db_index = sqlite3_column_int(s, 0);
        sqlite3 *sub_db = db_for_newsgroup(group);
        restore_postsets_from_db(sub_db, group);
        sqlite3_close(sub_db);
        group->is_subscribed = 1;
    }
    sqlite3_finalize(s);
}

void restore_postsets_from_db(sqlite3 *db, NewsGroup *group)
{
    sqlite3_stmt *s;
    string stmt = "SELECT postset_no, name FROM post_sets";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);
    sqlite3_bind_int(s, 1, group->db_index); 
    while (SQLITE_ROW == sqlite3_step(s)){
        PostSet *set = group->postset_for_subject((char *)sqlite3_column_text(s, 1));
        set->db_index = sqlite3_column_int(s, 0);
        restore_postfiles_from_db(db, set);
        set->has_msg_ids = 1;
    }
    sqlite3_finalize(s);
}

void restore_postfiles_from_db(sqlite3 *db, PostSet *set)
{
    sqlite3_stmt *s;
    string stmt = "SELECT postfile_no, name FROM post_files WHERE postset_no = ?";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);
    sqlite3_bind_int(s, 1, set->db_index); 
    while (SQLITE_ROW == sqlite3_step(s)){
        PostFile *file = set->file((char *)sqlite3_column_text(s, 1));
        file->db_index = sqlite3_column_int(s, 0);
    }
    sqlite3_finalize(s);
}
        
void restore_ids_from_db(sqlite3 *db, PostFile *file)
{
    sqlite3_stmt *s;
    string stmt = "SELECT msg_id, status FROM file_pieces WHERE postfile_no = ? ORDER BY file_piece_no";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);
    sqlite3_bind_int(s, 1, file->db_index); 
    while (SQLITE_ROW == sqlite3_step(s)){
        file->pieces.push_back(sqlite3_column_int(s, 1));
        file->piece_status.push_back((PIECE_STATUS) sqlite3_column_int(s, 2));
    }
    file->has_db_pieces = 1;
    sqlite3_finalize(s);
}
    
void save_postsets_to_db(sqlite3 *db, NewsGroup *group)
{
    sqlite3_stmt *s;
    string stmt = "INSERT INTO post_sets VALUES(?, ?)";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);

    Uint32 max_no = group->postsets.size();
    for(Uint32 i=0; i<max_no; i++){
        PostSet *set = group->postsets[i];
        sqlite3_bind_int(s, 1, i);
        sqlite3_bind_text(s, 2, set->subject.c_str(), set->subject.length(), NULL);
        sqlite3_step(s);
        sqlite3_reset(s);
        set->db_index = sqlite3_last_insert_rowid(db);
        save_postfiles(db, set);
    }
    sqlite3_finalize(s);
}

void setup_newsgroup_tables(sqlite3 *db)
{
    vector<string> queries;
    queries.push_back("CREATE TABLE newsgroups  (newsgroup_no  INTEGER PRIMARY KEY, name VARCHAR)");
    run_queries(db, queries);
}

void setup_postset_tables(sqlite3 *db)
{
    vector<string> queries;
    queries.push_back("CREATE TABLE post_sets   (postset_no    INTEGER PRIMARY KEY, name VARCHAR)");
    queries.push_back("CREATE TABLE post_files  (postfile_no   INTEGER PRIMARY KEY, postset_no INTEGER, name VARCHAR)");
    queries.push_back("CREATE TABLE file_pieces (file_piece_no INTEGER PRIMARY KEY, postfile_no INTEGER, status INTEGER, msg_id INTEGER)");
    run_queries(db, queries);
}

void run_queries(sqlite3 *db, vector<string> queries)
{
    Uint32 max_no = queries.size();
    for(Uint32 i=0; i<max_no; ++i) {
        sqlite3_exec(db, queries[i].c_str(), NULL, NULL, NULL);
    }
}

void save_subscribed_groups_to_db(sqlite3* db)
{
    sqlite3_stmt *s;
    string stmt = "INSERT INTO newsgroups VALUES(NULL, ?)";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);

    Uint32 max_no = newsgroups.size();
    for(Uint32 i=0; i<max_no; i++){
        NewsGroup *group = newsgroups[i];
        if(group->is_subscribed){
            sqlite3_bind_text(s, 1, group->name.c_str(), group->name.length(), NULL);
            sqlite3_step(s);
            sqlite3_reset(s);
            group->db_index = sqlite3_last_insert_rowid(db);

            sqlite3 *sub_db = db_for_newsgroup(group);
            sqlite3_exec(sub_db, "BEGIN TRANSACTION", NULL, NULL, NULL);
            save_postsets_to_db(sub_db, group);
            sqlite3_exec(sub_db, "COMMIT TRANSACTION", NULL, NULL, NULL);
            sqlite3_close(sub_db);
        }
    }
    sqlite3_finalize(s);
}

void save_postfiles(sqlite3 *db, PostSet *set)
{
    sqlite3_stmt *pf;
    string pf_stmt = "INSERT INTO post_files VALUES(NULL, ?, ?)";
    sqlite3_prepare(db, pf_stmt.c_str(), pf_stmt.length(), &pf, 0);
    Uint32 max_no = set->files.size();
    for(Uint32 i=0; i<max_no; i++){
        PostFile *file = set->files[i];
        sqlite3_bind_int(pf, 1, set->db_index); 
        sqlite3_bind_text(pf, 2, file->filename.c_str(), file->filename.length(), NULL);
        sqlite3_step(pf);
        sqlite3_reset(pf);
        file->db_index = sqlite3_last_insert_rowid(db);
        save_ids_to_db(db, file);
    }
    sqlite3_finalize(pf);
}

void save_ids_to_db(sqlite3* db, PostFile *file)
{
    sqlite3_stmt *fp;
    string fp_stmt = "INSERT INTO file_pieces VALUES(NULL, ?, ?, ?)";
    sqlite3_prepare(db, fp_stmt.c_str(), fp_stmt.length(), &fp, 0);
    Uint32 max_no = file->pieces.size();
    for(Uint32 i=0; i<max_no; ++i){
        sqlite3_bind_int(fp, 1, file->db_index); 
        sqlite3_bind_int(fp, 2, file->piece_status[i]); 
        sqlite3_bind_int(fp, 3, file->pieces[i]); 
        sqlite3_step(fp);
        sqlite3_reset(fp);
    }
    sqlite3_finalize(fp);
}

void restore_ids_from_db(PostFile *file)
{
    if(!file->db_index || file->has_db_pieces)
        return;
    sqlite3 *db = db_for_newsgroup(file->post_set->group);

    restore_ids_from_db(db, file);
    sqlite3_close(db);
}

Uint32 db_file_exists(string filename)
{
    struct stat buf;
    return -1 != stat(filename.c_str(), &buf);
}

void remove_postset_info_from_db(PostSet *set)
{
}

sqlite3 *db_for_newsgroup(NewsGroup *group)
{
    sqlite3 *db;
    string filename = config->blackbeard_data_dir 
                      + "/" + group->name;
    Uint32 existed = db_file_exists(filename);

    int rc = sqlite3_open(filename.c_str(), &db);
    console->log("Database file: " + filename);
    if(rc != SQLITE_OK){
        console->log("Could not create or open database " + filename);
    }

    if(!existed){
        console->log("Setting up postset tables for group " + group->name);
        setup_postset_tables(db);
    }
    return db;
}

