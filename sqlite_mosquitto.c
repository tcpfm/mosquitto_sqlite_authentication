#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "sqlite_mosquitto.h"
#include "utils.h"

sqlite3 *db;
sqlite3_stmt *res;
static const char *sql_create_table_mqtt_users = "CREATE TABLE IF NOT EXISTS mqtt_users(" \
        "id INTEGER PRIMARY KEY AUTOINCREMENT," \
        "clientid TEXT UNIQUE NOT NULL," \
        "username TEXT UNIQUE NOT NULL," \
        "password TEXT NOT NULL);" \
        "INSERT INTO mqtt_users (clientid,username,password) " \
        "VALUES ('clientid','admin','password');";

const char *sql_select_user_statement_template = " SELECT id,%s,%s FROM " \
        "%s WHERE %s = ? AND %s = ?";

// static const char *sql_select_statement_template = "SELECT id,";
// static const char *sql_from_statement_template = " FROM ";
// static const char *sql_where_statement_template = " WHERE ";
// static const char *sql_and_statement_template = " AND ";
// static const char *sql_question_statement_template = " = ? ";
// static const char *sql_comma_statement_template = ",";
//"WHERE username = ? AND password = ?";



static const char *sql_insert_admin = "INSERT INTO mqtt_users (username,password) " \
        "VALUES (admin,admin);";

void get_sqlite_version(void) {
    print(4, "SQLITE plugin version %s\n", sqlite3_libversion());
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i<argc; i++) {
        print(4, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
   return 0;
}

int open_db(database_t *database) {
    
    int sql_open_db_ret = sqlite3_open(database->database_file, &db);
    switch (sql_open_db_ret) {
    case SQLITE_OK:
        print(4, "Opened database successfully from %s", database->database_file);
        break;
    default:
        print(3, "Can't open database %s: %s",database->database_file, sqlite3_errmsg(db));
        return 1;
        break;
    }
    return 0;
}

int create_table_mqtt_users(database_t *database) {
    char *zErrMsg = 0;
    int sql_create_db_ret = sqlite3_exec(db, sql_create_table_mqtt_users, callback, 0, (char **)zErrMsg);

    switch (sql_create_db_ret) {
    case SQLITE_OK:
        print(4, "Table created successfully usign query: %s", sql_create_table_mqtt_users);
        break;
    case SQLITE_CONSTRAINT:
        print(4, "[DEBUG] %s", sqlite3_errmsg(db));
        break;

    default:
        print(3, "SQL error [%d]: %s", sqlite3_errcode(db), sqlite3_errmsg(db));
        sqlite3_free(zErrMsg);
        return 1;
        break;
    }
    return 0;
}

int init_db(database_t *database){
    print(4, "Starting sqlite authentication plugin.");
    get_sqlite_version();
    if (open_db(database) == 0) {
        create_table_mqtt_users(database);
    }
    
    // sql_select_user_table= malloc(sizeof(char *) * \
    //     (strlen(database->username_field) + strlen(database->password_field) + \
    //     strlen(sql_select_statement_template) + strlen(sql_from_statement_template)) + \
    //     strlen(sql_where_statement_template) + strlen(sql_and_statement_template) + \
    //     strlen(database->table_name) + strlen(sql_question_statement_template) + \
    //     strlen(sql_comma_statement_template));
    return 0;
}
 

int authenticate_user(user_t *user, database_t *database) {
    print(4, "Starting authentication with username and password.");

    /*
     * ALWAYS when call the function concatenate_by_args, you must (MUST)
     * pass the last parameter as NULL. It will return garbage otherwise
    */
    char *sql_select_user_statement_template2 = concatenate_by_args( \
            sql_select_user_statement_template, \
            database->username_field, database->password_field, database->table_name, \
            database->username_field, database->password_field, NULL);
    int sql_select_user_ret = sqlite3_prepare_v2(db, \
            sql_select_user_statement_template2, -1, &res, 0);

    sql_select_user_statement_template2 = NULL;

    // this functions will free the malloced variables
    dispose_utils();

    switch (sql_select_user_ret) {
    case SQLITE_OK:
        /*
         * These binds represents the order specified in SQL SELECT QUERY
         * the first ? symbol represents 1
         * the second ? symbol represents 2
        */
        sqlite3_bind_text(res, 1, user->username, strlen(user->username), NULL);
        sqlite3_bind_text(res, 2, user->password, strlen(user->password), NULL);
        break;
    
    default:
        print(3, "Failed fetch username and password from database: %s", sqlite3_errmsg(db));
        return 1;
        break;
    }
    int step = sqlite3_step(res);
       
   if (step == SQLITE_ROW) {
        if (strncmp(user->username, (const char*)sqlite3_column_text(res, 1), strlen(user->username)) == 0){
            if (strncmp(user->password, (const char *)sqlite3_column_text(res, 2), strlen(user->password)) == 0){
                user->id = sqlite3_column_int(res, 0);
                return USER_AUTHENTICATED;
            }
        }
    }
    return USER_USERNAME_OR_PASSWORD_INCORRECT;
}

void dispose_db(void) {
    print(4, "Terminating sqlite plugin.");
    //free(sql_select_user_table);
    sqlite3_reset(res);
    sqlite3_finalize(res);
    sqlite3_close(db);
    sqlite3_free(0);
    dispose_utils();
}
