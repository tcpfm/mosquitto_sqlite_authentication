#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite_mosquitto.h"
#include "utils.h"

sqlite3 *db;
int sqlopen;
sqlite3_stmt *res;


void get_sqlite_version(void) {
    print(4, "SQLITE plugin version %s\n", sqlite3_libversion());
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int init_db(database_t *database){
    print(4, "Starting sqlite authentication plugin.");

    get_sqlite_version();

    char *zErrMsg = 0;
     sqlopen = sqlite3_open(database->database_file, &db);

    if ( sqlopen ) {
        print(3, "Can't open database %s: %s",database->database_file, sqlite3_errmsg(db));
        return 1;
    } else {
        print(4, "Opened database successfully from %s", database->database_file);
    }
    

    sqlopen = sqlite3_exec(db, sql_create_table, callback, 0, (char **)zErrMsg);

    if( sqlopen != SQLITE_OK ){
        print(3, "SQL error [%d]: %s", sqlite3_errcode(db), sqlite3_errmsg(db));
        sqlite3_free(zErrMsg);
        return 1;
    } else {
        print(4, "Table created successfully usign query: %s", sql_create_table);
    }
    return 0;
}

int authenticate_user(user_t *user) {
    print(4, "Starting authentication with username and password.");

    sqlopen = sqlite3_prepare_v2(db, sql_select_user, -1, &res, 0);

    if (sqlopen == SQLITE_OK) {
        sqlite3_bind_text(res, 1, user->username, strlen(user->username), NULL);
        sqlite3_bind_text(res, 2, user->password, strlen(user->password), NULL);
    } else {
        print(3, "Failed fetch username and password from database: %s", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(res);
       
   if (step == SQLITE_ROW) {
        if (strncmp(user->username, (const char*)sqlite3_column_text(res, 0), strlen(user->username)) == 0){
            if (strncmp(user->password, (const char *)sqlite3_column_text(res, 1), strlen(user->password)) == 0){
                return USER_AUTHENTICATED;
            }
        }
    }
    return USER_USERNAME_OR_PASSWORD_INCORRECT;
}

void dispose_db(void) {
    print(4, "Terminating sqlite plugin.");
    sqlite3_reset(res);
    sqlite3_finalize(res);
    sqlite3_close(db);
    sqlite3_free(0);
}
