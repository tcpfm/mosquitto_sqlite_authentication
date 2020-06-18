#include <json-c/json.h>
#include <string.h>
#include "json.h"
#include "utils.h"

void parse_to_manage_users(manage_user_t **manage_user, char *json) {
    json_object *json_obj = json_tokener_parse(json);
    manage_user_t *user = malloc(sizeof(manage_user_t));
    enum json_type type;
    json_object_object_foreach(json_obj, key, value) {
        type = json_object_get_type(value);
        switch (type) {
        case json_type_null:
            print(4, "JSON type is null.");
            break;
        case json_type_string:
            if (strncmp(key, "username", strlen(key)) == 0)user->username = json_object_get_string(value);
            if (strncmp(key, "password", strlen(key)) == 0)user->password = json_object_get_string(value);
            if (strncmp(key, "action", strlen(key)) == 0)user->action = json_object_get_string(value);
            break;
        case json_type_boolean:
            if (strncmp(key, "is_admin", strlen(key)) == 0)user->is_admin = json_object_get_boolean(value);
            break;
        default:
            break;
        }
        *manage_user = user;
    }
    user = NULL;
    manage_user = NULL;
}