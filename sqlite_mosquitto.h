#ifndef _SQLITE_MOSQUITTO_H_
#define _SQLITE_MOSQUITTO_H_

#define USER_AUTHENTICATED 0
#define USER_USERNAME_OR_PASSWORD_INCORRECT 1
#define USER_HAS_NO_PERMITION 3
#define USER_UNKNOWN 4


typedef struct {
    const char *database_file;
    const char *table_name;
    const char *username_field;
    const char *password_field;
} database_t;

typedef struct {
    int id;
    const char *username;
    const char *password;
} user_t;

void get_sqlite_version(void);
int init_db(database_t *database);
void dispose_db(void);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
int authenticate_user(user_t *user, database_t *database);
int open_db(database_t *database);
int create_table_mqtt_users(database_t *database);

#endif