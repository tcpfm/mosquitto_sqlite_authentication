#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "utils.h"



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

void concatenate_by_args(char **ret, const char *format, ...) {
    
    int args_size = strlen(format);
    if (args_size <= 0) return;
    va_list args;
    va_list args_cpy;
    va_start(args, format);
    
    va_copy(args_cpy, args);
    char *arg = NULL;
    char *temp_args;

    while ((arg = va_arg(args, char *)) != NULL) args_size += strlen(arg);
    
    temp_args = malloc(sizeof(char *) * args_size +1);
    vsnprintf(temp_args, args_size,format, args_cpy);
    print(4, "Copying content to variable before malloc(), length: %d on memory address: %u", \
            strlen(temp_args), temp_args);

    va_end(args);
    va_end(args_cpy);
    *ret = temp_args;
    print(4, "Setting return value to variable passed byref on address %u", ret);
    temp_args = NULL;
    ret = NULL;
    print(4, "Confirming that variables passed byref was null on adresses %u and %u", \
            temp_args, ret);
}