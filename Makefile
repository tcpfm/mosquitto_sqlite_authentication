.PHONY: all clean

DEBUG_OPT=-DMQAP_DEBUG
CFLAGS=-Wall -Werror -Wno-unused-function -Wno-unused-variable -Wno-comment -ggdb -l sqlite3 -ldl

all : sqlite_auth_plugin.so 

sqlite_auth_plugin.so : sqlite_auth_plugin.c
	$(CC) ${CFLAGS} -fPIC -shared $^ -o $@ 

debug: sqlite_auth_plugin_debug.so

sqlite_auth_plugin_debug.so : sqlite_auth_plugin.c
	$(CC) ${CFLAGS} ${DEBUG_OPT} -fPIC -shared *.c *.h -o sqlite_auth_plugin_debug.so


clean :
	rm -f *.so *.test