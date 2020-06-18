#ifndef _JSON_H_
#define _JSON_H

typedef struct {
    const char *username;
    const char *password;
    const char *action;
    int is_admin;
} manage_user_t;

void parse_to_manage_users(manage_user_t **manage_user, char *json);
#endif