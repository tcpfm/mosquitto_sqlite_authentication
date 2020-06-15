#include "sqlite_auth_plugin.h"
#include <string.h>
#include <malloc.h>
#include "sqlite_mosquitto.h"

database_t *database;
user_t *user;

int mosquitto_auth_plugin_version(void) {
    fprintf(stdout, "Starting mosquitto plugin version %d\n", MOSQ_AUTH_PLUGIN_VERSION);
    return MOSQ_AUTH_PLUGIN_VERSION;
}
int mosquitto_auth_plugin_init(void **user_data, 
        struct mosquitto_opt *opts, 
        int opt_count
        ) {
    database = (database_t *)malloc(sizeof(database_t));
    user = (user_t *)malloc(sizeof(user_t));

    fprintf(stdout, "MOSQUITO_AUTH_PLUGIN_INIT\n");
    if (opt_count > 0){
        for (int i = 0; i < opt_count; i++) {
            if (strncmp(opts[i].key, "sqlite_database_file", 20) == 0) {
                fprintf(stdout, "Loading sqlite database file: %s\n", opts[i].value);
                database->database_file = opts[i].value;
            } else if (strncmp(opts[i].key, "sqlite_database_username_field", 30) == 0) {
                database->username_field = opts[i].value;
                fprintf(stdout, "Loading sqlite query username field: %s\n", database->username_field);
            } else if (strncmp(opts[i].key, "sqlite_database_password_field", 30) == 0) {
                database->password_field = opts[i].value;
                fprintf(stdout, "Loading sqlite query password field: %s\n", database->password_field);
            }
        } 
    } else {
        fprintf(stderr, "There is no options to load sqlite authentication plugin.\n");
        return 1;
    }
    //init_db();
    return 0;
}

int mosquitto_auth_plugin_cleanup(void *user_data, 
        struct mosquitto_opt
        *opts, int opt_count
        ) {

    fprintf(stdout, "MOSQUITTO_AUTH_PLUGIN_CLEANUP\n");
    free(user);
    free(database);
    //dispose_db(void);
    return 0;
}

int mosquitto_auth_security_init(void *user_data, 
        struct mosquitto_opt *opts, 
        int opt_count, 
        bool reload
        ){
    fprintf(stdout, "MOSQUITTO_AUTH_SECURITY_INIT\n");
    return 0;
}

int mosquitto_auth_security_cleanup(void *user_data, 
        struct mosquitto_opt *opts, 
        int opt_count, 
        bool reload
        ) {
    
    fprintf(stdout, "MOSQUITTO_AUTH_SECURITY_CLENAUP\n");
    return 0;
}

int mosquitto_auth_acl_check(void *user_data, 
        int access, 
        const struct mosquitto *client, 
        const struct mosquitto_acl_msg *msg
        ) {
    if (strncmp(msg->topic, "/teste", 5) == 0) {
        if (access == MOSQ_ACL_SUBSCRIBE) {
            fprintf(stdout, "Your subscription was rejected by server on topic %s for user %s\n", msg->topic, user->username);
        } else {
            fprintf(stdout, "Your publishing was rejected by server on topic %s for user %s\n", msg->topic, user->username);
        }
        return MOSQ_ERR_ACL_DENIED;
    }else {
        fprintf(stdout, "MOSQUITTO_AUTH_ACL_CHECK \"%i\"\n", access);
        return MOSQ_ERR_SUCCESS;
    }
}

int mosquitto_auth_unpwd_check(void *user_data, 
        const struct mosquitto *client, 
        const char *username, 
        const char *password
        ) {
    user->username = username;
    user->password = password;
    if (authenticate_user(user) == USER_AUTHENTICATED){
        fprintf(stdout, "User successfuly authenticated.\n");    
    }
    fprintf(stdout, "MOSQUITTO_AUTH_UNPWD_CHECK\n");
    if (username == NULL) {
        fprintf(stderr, "Username must be provided.\n");
        return MOSQ_ERR_ACL_DENIED;
    }else if (password == NULL) {
        fprintf(stderr, "Password must be provided.\n");
        return MOSQ_ERR_ACL_DENIED;
    }
    
    return 0;
}

int mosquitto_auth_psk_key_get(void *user_data, 
        const struct mosquitto *client, 
        const char *hint, 
        const char *identity, 
        char *key, 
        int max_key_len
        ) {
	fprintf(stdout, "MOSQUITTO_AUTH_PSK_KEY_GET\n");
    return 0;
}

int mosquitto_auth_start(void *user_data, 
        struct mosquitto *client, 
        const char *method, 
        bool reauth, 
        const void *data_in, 
        uint16_t data_in_len, 
        void **data_out, 
        uint16_t *data_out_len
        ) {
    fprintf(stderr, "MOSQUITTO_AUTH_START\n");
    return 0;
}
