#ifndef _SQLITE_MOSQUITTO_H_
#define _SQLITE_MOSQUITTO_H_

#include <sqlite3.h>

#define USER_AUTHENTICATED 0
#define USER_USERNAME_OR_PASSWORD_INCORRECT 1
#define USER_HAS_NO_PERMITION 3
#define USER_UNKNOWN 4


typedef struct {
    const char *database_file;
    const char *username_field;
    const char *password_field;
} database_t;

typedef struct {
    const char *username;
    const char *password;
} user_t;

void get_sqlite_version(void);
int init_db(database_t *database);
void dispose_db(void);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
int authenticate_user(user_t *user);

static const char *sql_create_table = "CREATE TABLE IF NOT EXISTS mqtt_users(" \
        "id INTEGER PRIMARY KEY AUTOINCREMENT," \
        "clientid TEXT UNIQUE NOT NULL," \
        "username TEXT UNIQUE NOT NULL," \
        "password TEXT NOT NULL);" \
        "INSERT INTO mqtt_users (clientid,username,password) " \
        "VALUES ('clientid','admin','password');";

static const char *sql_select_user = "SELECT username,password "\
        "FROM mqtt_users "\
        "WHERE username = ? AND password = ?;";

static const char *sql_insert_admin = "INSERT INTO mqtt_users (username,password) " \
        "VALUES (admin,admin);";

#endif