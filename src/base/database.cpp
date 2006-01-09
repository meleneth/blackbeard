#include "database.hpp"
#include "config.hpp"
#include "console.hpp"

void save_db_data()
{
    string filename = config->blackbeard_data_dir + "/blackbeard.db";
    int rc;
    sqlite3* db;

    rc = sqlite3_open(filename.c_str(), &db);
    console->log("Database file: " + filename);
    if(rc != SQLITE_OK){
        console->log("Could not create database " + filename);
    }
    setup_newsgroup_tables(db);
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    save_subscribed_groups_to_db(db);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
}

void restore_db_data()
{
    console->log("Restoring.. but no code written");
}

void save_postsets_to_db(sqlite3 *db, NewsGroup *group)
{
    sqlite3_stmt *s;
    string stmt = "INSERT INTO post_sets VALUES(?, ?, ?)";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);

    Uint32 max_no = group->postsets.size();
    for(Uint32 i=0; i<max_no; i++){
        PostSet *set = group->postsets[i];
        sqlite3_bind_int(s, 1, i); 
        sqlite3_bind_int(s, 2, group->index()); 
        sqlite3_bind_text(s, 3, set->subject.c_str(), set->subject.length(), NULL);
        sqlite3_step(s);
        sqlite3_reset(s);

        save_postfiles(db, set);
    }
    sqlite3_finalize(s);
}

void setup_newsgroup_tables(sqlite3 *db)
{
    vector<string> queries;
    queries.push_back("CREATE TABLE newsgroups (newsgroup_no INTEGER, name VARCHAR)");
    queries.push_back("CREATE TABLE post_files (postfile_no INTEGER, postset_no INTEGER, name VARCHAR)");
    queries.push_back("CREATE TABLE post_sets (postset_no INTEGER, newsgroup_no INTEGER, name VARCHAR)");
    queries.push_back("CREATE TABLE file_pieces (file_piece_no INTEGER, postfile_no INTEGER, status INTEGER, msg_id INTEGER)");
    Uint32 max_no = queries.size();
    for(Uint32 i=0; i<max_no; ++i) {

       // int rc = 
        sqlite3_exec(db, queries[i].c_str(), NULL, NULL, NULL);
        /*if(rc != SQLITE_OK){
            stringstream s;
            s << "Death - " << rc << " - :(";
            console->log(s.str());
        }*/
    }
}

void save_subscribed_groups_to_db(sqlite3* db)
{
    sqlite3_stmt *s;
    string stmt = "INSERT INTO newsgroups VALUES(?, ?)";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);

    Uint32 max_no = newsgroups.size();
    for(Uint32 i=0; i<max_no; i++){
        NewsGroup *group = newsgroups[i];
        if(group->is_subscribed){
            sqlite3_bind_int(s, 1, i); 
            sqlite3_bind_text(s, 2, group->name.c_str(), group->name.length(), NULL);
            sqlite3_step(s);
            sqlite3_reset(s);
            save_postsets_to_db(db, group);
        }
    }
    sqlite3_finalize(s);
}

void save_postfiles(sqlite3 *db, PostSet *set)
{
    sqlite3_stmt *pf;
    string pf_stmt = "INSERT INTO post_files VALUES(?, ?, ?)";
    sqlite3_prepare(db, pf_stmt.c_str(), pf_stmt.length(), &pf, 0);
    Uint32 max_no = set->files.size();
    for(Uint32 i=0; i<max_no; i++){
        PostFile *file = set->files[i];
        sqlite3_bind_int(pf, 1, i); 
        sqlite3_bind_int(pf, 2, set->index()); 
        sqlite3_bind_text(pf, 3, file->filename.c_str(), file->filename.length(), NULL);
        sqlite3_step(pf);
        sqlite3_reset(pf);
        save_ids_to_db(db, file);
    }
    sqlite3_finalize(pf);
}

void save_ids_to_db(sqlite3* db, PostFile *file)
{
    sqlite3_stmt *fp;
    string fp_stmt = "INSERT INTO file_pieces VALUES(?, ?, ?, ?)";
    sqlite3_prepare(db, fp_stmt.c_str(), fp_stmt.length(), &fp, 0);
    Uint32 max_no = file->pieces.size();
    for(Uint32 i=0; i<max_no; ++i){
        sqlite3_bind_int(fp, 1, i); 
        sqlite3_bind_int(fp, 2, file->index()); 
        sqlite3_bind_int(fp, 3, file->piece_status[i]); 
        sqlite3_bind_int(fp, 4, file->pieces[i]); 
        sqlite3_step(fp);
        sqlite3_reset(fp);
    }
    sqlite3_finalize(fp);
}

