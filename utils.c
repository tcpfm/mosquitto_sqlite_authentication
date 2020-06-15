#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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