#include "database.hpp"
#include "config.hpp"
#include "console.hpp"
#include "mnzb.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>

using std::stringstream;

sqlite3 *get_main_db(void)
{
    string filename = config->blackbeard_data_dir + "/blackbeard.db";
    int rc;
    sqlite3* db;

    Uint32 existed = db_file_exists(filename);
    rc = sqlite3_open(filename.c_str(), &db);
    console->log("Database file: " + filename);
    if(rc != SQLITE_OK){
        console->log("Could not create database " + filename);
    }
    if(!existed)
        setup_newsgroup_tables(db);
    return db;
}

void save_db_data()
{
    sqlite3* db = get_main_db();

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    save_subscribed_groups_to_db(db);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    sqlite3_close(db);
}

void restore_db_data()
{
    sqlite3* db = get_main_db();
    restore_newsgroups_from_db(db);
    sqlite3_close(db);

}

void save_newsgroup_to_db(NewsGroup *group)
{
    sqlite3* db = get_main_db();

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    save_subscribed_groups_to_db(db);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
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
        sqlite3_stmt *q;
        stmt = "select min(article_no), max(article_no) from file_pieces where article_no != 1";
        sqlite3_prepare(sub_db, stmt.c_str(), stmt.length(), &q, 0);
        if(SQLITE_ROW == sqlite3_step(q)){
            group->first_article_number = sqlite3_column_int(q, 0);
            group->last_article_number = sqlite3_column_int(q, 1);
        }
        sqlite3_finalize(q);

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
        set->tick = 1;
    }
    sqlite3_finalize(s);
}

void restore_postfiles(PostSet *set)
{
        mNZB nzb;
        nzb.load_postset(set);
}
        

void delete_old_postsets(sqlite3 *db, NewsGroup *group)
{
    sqlite3_stmt *s;
    string postset_delete = "DELETE FROM post_sets WHERE postset_no IN (SELECT postset_no FROM post_files WHERE postfile_no IN (SELECT DISTINCT(postfile_no) FROM file_pieces WHERE article_no < ?))";

    sqlite3_prepare(db, postset_delete.c_str(), postset_delete.length(), &s, 0);
    sqlite3_bind_int(s, 1, group->first_article_number);
    sqlite3_step(s);
    sqlite3_finalize(s);
}
    
void save_postsets_to_db(sqlite3 *db, NewsGroup *group)
{
    delete_old_postsets(db, group);
    sqlite3_stmt *s;
    string stmt = "INSERT INTO post_sets VALUES(?, ?, ?, ?, ?";
    sqlite3_prepare(db, stmt.c_str(), stmt.length(), &s, 0);

    Uint32 max_no = group->postsets.size();
    for(Uint32 i=0; i<max_no; i++){
        PostSet *set = group->postsets[i];
        if(!set->db_index){
            console->log("INSERT INTO POST_SETS");
            sqlite3_bind_int(s, 1, i);
            sqlite3_bind_int(s, 2, set->num_bytes);
            sqlite3_bind_int(s, 3, set->max_article_no());
            sqlite3_bind_int(s, 4, set->min_article_no());
            sqlite3_bind_text(s, 5, set->subject.c_str(), set->subject.length(), NULL);
            sqlite3_step(s);
            sqlite3_reset(s);
            set->db_index = sqlite3_last_insert_rowid(db);
        } else {
            console->log("ALREADY HAD DB_INDEX");
        }
        mNZB nzb;
        nzb.save_postset(set);
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
    queries.push_back("CREATE TABLE post_sets (postset_no INTEGER PRIMARY KEY, num_bytes INTEGER, max_article_no INTEGER, min_article_no INTEGER, name VARCHAR)");
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
            if(!group->db_index){
                sqlite3_bind_text(s, 1, group->name.c_str(), group->name.length(), NULL);
                sqlite3_step(s);
                sqlite3_reset(s);
                group->db_index = sqlite3_last_insert_rowid(db);
            }

            sqlite3 *sub_db = db_for_newsgroup(group);
            sqlite3_exec(sub_db, "BEGIN TRANSACTION", NULL, NULL, NULL);
            save_postsets_to_db(sub_db, group);
            sqlite3_exec(sub_db, "COMMIT TRANSACTION", NULL, NULL, NULL);
            sqlite3_close(sub_db);
        }
    }
    sqlite3_finalize(s);
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
    string dirname = config->blackbeard_data_dir + "/" + group->name;
    string db_filename = dirname + ".db";
    Uint32 existed = db_file_exists(db_filename);

    int rc = sqlite3_open(db_filename.c_str(), &db);
    console->log("Database file: " + db_filename);
    if(rc != SQLITE_OK){
        console->log("Could not create or open database " + db_filename);
    }

    if(!existed){
        console->log("Setting up postset tables for group " + group->name);
        setup_postset_tables(db);
    }
    return db;
}

