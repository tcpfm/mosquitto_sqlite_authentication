#ifndef _SQLITE_MOSQUITTO_H_
#define _SQLITE_MOSQUITTO_H_

#include <sqlite3.h>

#define USER_AUTHENTICATED 0
#define USER_PASSWORD_INCORRECT 1
#define USER_USERNAME_INCORRECT 2
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
void init_db(void);
void dispose_db(void);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
int authenticate_user(user_t *user);

#endif