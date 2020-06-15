#include <stdio.h>
#include <string.h>
#include "sqlite_mosquitto.h"

sqlite3 *db;
char *sql;

void get_sqlite_version(void) {
    fprintf(stdout, "sqlite lib version %s\n", sqlite3_libversion());
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void init_db(void){
    fprintf(stdout, "Starting sqlite plugin.\n");

    get_sqlite_version();

    char *zErrMsg = 0;
    int sqlopen = sqlite3_open("db.sqlite", &db);

    if ( sqlopen ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    sql = "CREATE TABLE mqtt_users(" \
        "id INT PRIMARY KEY NOT NULL," \
        "clientid TEXT NOT NULL," \
        "username TEXT NOT NULL," \
        "password TEXT NOT NULL);";

    sqlopen = sqlite3_exec(db, sql, callback, 0, (char **)zErrMsg);

    if( sqlopen != SQLITE_OK ){
        fprintf(stderr, "SQL error [%d]: %s\n", sqlite3_errcode(db), sqlite3_errmsg(db));
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

int authenticate_user(user_t *user) {
    fprintf(stdout, "Username %s\nPassword %s\n", user->username, user->password);

    if (strncmp(user->username, "leo", 3) == 0) {
        if (strncmp(user->password, "leo", 3) == 0) {
            return USER_AUTHENTICATED;
        } else {
            return USER_PASSWORD_INCORRECT;
        } 
    }else {
            return USER_USERNAME_INCORRECT;
    }
    return USER_UNKNOWN;
}

void dispose_db(void) {
    fprintf(stdout, "Terminating sqlite plugin.\n");
    sqlite3_close(db);
    sqlite3_free(0);
}
