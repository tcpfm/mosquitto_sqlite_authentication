#include "sqlite_auth_plugin.h"
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "sqlite_mosquitto.h"
#include "utils.h"

database_t *database;
user_t *user;

int mosquitto_auth_plugin_version(void) {
    fprintf(stdout, "Starting mosquitto plugin version %d\n", MOSQ_AUTH_PLUGIN_VERSION);
    print(1, "Initializing the plugin in C: 1. %s, 2. %d, 3. %f" ,"primeiro", 2, 3.0);
    return MOSQ_AUTH_PLUGIN_VERSION;
}
int mosquitto_auth_plugin_init(void **user_data, 
        struct mosquitto_opt *opts, 
        int opt_count
        ) {
    database = malloc(sizeof(database_t));
    user = malloc(sizeof(user_t));

    fprintf(stdout, "MOSQUITO_AUTH_PLUGIN_INIT\n");
    if (opt_count > 0){
        for (int i = 0; i < opt_count; i++) {
            if (strncmp(opts[i].key, "sqlite_database_file", 20) == 0) {
                fprintf(stdout, "Loading sqlite database file: %s", opts[i].value);
                database->database_file = opts[i].value;
            } else if (strncmp(opts[i].key, "sqlite_database_username_field", 30) == 0) {
                database->username_field = opts[i].value;
                fprintf(stdout, "Loading sqlite query username field: %s", database->username_field);
            } else if (strncmp(opts[i].key, "sqlite_database_password_field", 30) == 0) {
                database->password_field = opts[i].value;
                fprintf(stdout, "Loading sqlite query password field: %s", database->password_field);
            }
        } 
    } else {
        print(3, "There is no options to load sqlite authentication plugin.");
        return 1;
    }
    init_db(database);
    return 0;
}

int mosquitto_auth_plugin_cleanup(void *user_data, 
        struct mosquitto_opt
        *opts, int opt_count
        ) {

    fprintf(stdout, "MOSQUITTO_AUTH_PLUGIN_CLEANUP\n");
    free(user);
    free(database);
    dispose_db();
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
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
            print(3, "Your subscription was rejected by server on topic %s for user %s", msg->topic, user->username);
        } else {
            print(3, "Your publishing was rejected by server on topic %s for user %s", msg->topic, user->username);
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

    fprintf(stdout, "MOSQUITTO_AUTH_UNPWD_CHECK\n");
    if (username == NULL) {
        print(2, "Username must be provided.");
        return MOSQ_ERR_ACL_DENIED;
    }else if (password == NULL) {
        print(2, "Password must be provided.");
        return MOSQ_ERR_ACL_DENIED;
    }

    if (authenticate_user(user) == USER_AUTHENTICATED){
        print(1, "User successfuly authenticated.");
        return MOSQ_ERR_SUCCESS;  
    } 
    print(2, "The provided username or password is incorrect.");
    return MOSQ_ERR_ACL_DENIED;
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
