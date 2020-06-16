#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "utils.h"

char *temp_args;

void print(int level, const char *message, ...) {
    va_list args;
    va_start(args, message);
    switch (level) {
    case 1:
        vfprintf(stdout, ANSI_COLOR_BLUE "[INFO] ", NULL);
        vfprintf(stdout, message, args);
        vfprintf(stdout, ANSI_COLOR_RESET "\n", NULL);
        break;
    case 2:
        vfprintf(stderr, ANSI_COLOR_YELLOW "[WARN] ", NULL);
        vfprintf(stderr, message, args);
        vfprintf(stderr, ANSI_COLOR_RESET "\n", NULL);
        break;
    case 3:
        vfprintf(stderr, ANSI_COLOR_RED "[ERROR] ", NULL);
        vfprintf(stderr, message, args);
        vfprintf(stderr, ANSI_COLOR_RESET "\n", NULL);
        break;
#ifdef MQAP_DEBUG
    case 4:
        vfprintf(stdout, ANSI_COLOR_CYAN "[DEBUG] ", NULL);
        vfprintf(stdout, message, args);
        vfprintf(stdout, ANSI_COLOR_RESET "\n", NULL);
        break;
#endif
    default:
        break;
    }
    
    va_end(args);
}

char *concatenate_by_args(const char *format, ...) {
    
    int args_size = strlen(format);
    if (args_size <= 0) return NULL;
    va_list args;
    va_list args_cpy;
    va_start(args, format);
    
    va_copy(args_cpy, args);
    char *arg = NULL;
    int x = 0;
    while ((arg = va_arg(args, char *)) != NULL) {
        print(3, "entrou no loog [%d] %s....", x++, arg);
        args_size += strlen(arg);
    }
    print(3, "chegou aqui....");
    if (temp_args != NULL) temp_args = NULL;
    temp_args = malloc(sizeof(char *) * args_size +1);
    
    vsnprintf(temp_args, args_size,format, args_cpy);
    va_end(args);
    va_end(args_cpy);
    return temp_args;
}

void dispose_utils(void) {
    temp_args = NULL;
    free(temp_args);
}