
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <mosquitto.h>
#include <mosquitto_plugin.h>
#include "sqlite_auth_plugin.h"
#include "sqlite_mosquitto.h"
#include "utils.h"
#include "json.h"

database_t  *database;
user_t      *user;
manage_user_t *manage_user;

char *manage_user_topic;
static const char *sqlite_database_file_path_key    = "sqlite_database_file_path";
static const char *sqlite_table_username_field_key  = "sqlite_table_username_field";
static const char *sqlite_table_password_field_key  = "sqlite_table_password_field";
static const char *sqlite_tsqlite_table_name_key    = "sqlite_table_name";
static const char *sqlite_manage_users_topic_key    = "sqlite_manage_users_topic";

int mosquitto_auth_plugin_version(void) {
    print(4, "FUNCTION: MOSQUITTO_AUTH_PLUGIN_VERSION");
    print(4, "Mosquitto authentication plugin version %d\n", MOSQ_AUTH_PLUGIN_VERSION);
    return MOSQ_AUTH_PLUGIN_VERSION;
}

int mosquitto_auth_plugin_init(void **user_data, 
        struct mosquitto_opt *opts, int opt_count) {

    database = malloc(sizeof(database_t));
    user = malloc(sizeof(user_t));

    print(4, "FUNCTION: MOSQUITO_AUTH_PLUGIN_INIT");
    if (opt_count > 0) {
        for (int i = 0; i < opt_count; i++) {
            if (strncmp(opts[i].key, sqlite_database_file_path_key, \
                        strlen(sqlite_database_file_path_key)) == 0) {
                print(4, "Loading sqlite database file: %s", opts[i].value);
                database->database_file = opts[i].value;
            } else
                if (strncmp(opts[i].key, sqlite_tsqlite_table_name_key, \
                        strlen(sqlite_tsqlite_table_name_key)) == 0) {
                    database->table_name = opts[i].value;
                    print(4, "Loading sqlite table name field: %s", database->table_name);
            }else 
                if (strncmp(opts[i].key, sqlite_table_username_field_key, \
                        strlen(sqlite_table_username_field_key)) == 0) {
                    database->username_field = opts[i].value;
                    print(4, "Loading sqlite query username field: %s", database->username_field);

            } else 
                if (strncmp(opts[i].key, sqlite_table_password_field_key, \
                        strlen(sqlite_table_password_field_key)) == 0) {
                    database->password_field = opts[i].value;
                    print(4, "Loading sqlite query password field: %s", database->password_field);
            } else
                if (strncmp(opts[i].key, sqlite_manage_users_topic_key, \
                        strlen(sqlite_manage_users_topic_key)) == 0) {
                    manage_user_topic = opts[i].value;
                    print(4, "Loading root topic to manage users: %s", manage_user_topic);
                }
            
        } 
    } else {
        print(3, "There is no options to load sqlite authentication plugin.");
        return 1;
    }
    if (init_db(database) != 0) {
        dispose_db();
        return 1;
    }
    return 0;
}

int mosquitto_auth_plugin_cleanup(void *user_data, 
        struct mosquitto_opt *opts, int opt_count) {

    print(4, "FUNCTION: MOSQUITTO_AUTH_PLUGIN_CLEANUP");
    free(user);
    free(database);
    dispose_db();
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
    return 0;
}

int mosquitto_auth_security_init(void *user_data, struct mosquitto_opt *opts, 
        int opt_count, bool reload){
    print(4, "FUNCTION: MOSQUITTO_AUTH_SECURITY_INIT");
    return 0;
}

int mosquitto_auth_security_cleanup(void *user_data, 
        struct mosquitto_opt *opts, int opt_count, bool reload) {
    print(4, "FUNCTION: MOSQUITTO_AUTH_SECURITY_CLENAUP");
    return 0;
}

int mosquitto_auth_acl_check(void *user_data, int access, 
        const struct mosquitto *client,  
        const struct mosquitto_acl_msg *msg) {
    print(4, "FUNCTION: MOSQUITTO_AUTH_ACL_CHECK [%s]", client);
    if (strncmp(msg->topic, manage_user_topic, strlen(manage_user_topic)) == 0) {
        if (access == MOSQ_ACL_SUBSCRIBE) {
            print(3, "Your subscription was rejected by server on topic %s for user %s", \
                msg->topic, user->username);
        } else {
            char *payload = malloc(sizeof(char *) * msg->payloadlen +1);
            snprintf(payload, msg->payloadlen +1, msg->payload);
            print(4, "Loading parameters: %s on topic: %s", \
                payload, msg->topic);
            parse_to_manage_users(&manage_user, payload);
            print(4, "Processed value: \n\t%s, \n\t%s, \n\t%s, \n\t%d", manage_user->username, \
            manage_user->password, manage_user->action, manage_user->is_admin);
            payload = NULL;
            manage_user = NULL;
        }
        return MOSQ_ERR_ACL_DENIED;
    }else {
        return MOSQ_ERR_SUCCESS;
    }
}

int mosquitto_auth_unpwd_check(void *user_data, const struct mosquitto *client, 
        const char *username, const char *password) {
    print(4, "FUNCTION: MOSQUITTO_AUTH_UNPWD_CHECK");
    user->username = username;
    user->password = password;
    
    if (username == NULL) {
        print(2, "Username must be provided.");
        return MOSQ_ERR_ACL_DENIED;
    } else 
        if (password == NULL) {
        print(2, "Password must be provided.");
        return MOSQ_ERR_ACL_DENIED;
    }
    if (authenticate_user(user, database) == USER_AUTHENTICATED){
        print(1, "User successfuly authenticated.");
        return MOSQ_ERR_SUCCESS;  
    }

    print(2, "The provided username or password is incorrect.");
    return MOSQ_ERR_ACL_DENIED;
}

int mosquitto_auth_psk_key_get(void *user_data, const struct mosquitto *client, 
        const char *hint, const char *identity, char *key, int max_key_len) {
	print(4, "FUNCTION: MOSQUITTO_AUTH_PSK_KEY_GET");
    return 0;
}

int mosquitto_auth_start(void *user_data, struct mosquitto *client, 
        const char *method, bool reauth, const void *data_in, 
        uint16_t data_in_len, void **data_out, uint16_t *data_out_len) {
    print(4, "FUNCTION: MOSQUITTO_AUTH_START");
    return 0;
}
