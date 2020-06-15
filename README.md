# Introdution
This project consists in a easy way to authenticate and control all of your IoT devices using a single and fast database

# Instalation

## Dependencies
First of all, you need to install some compilation dependencies to use the Makefile properly:

>```libsqlite3-dev```
>
>```libmosquitto-dev```
>
>```mosquitto-dev```

## Compilation
### All of the dependent libs are compiled as a shared lib, so you must have this same dev dependencies on your final location

You can also use some alias to your command make

Use ```make debug``` to compile a debug version and enable all of the logs output, use this option with carreful, it will generate a tons of log lines.

Use ```make all``` just to compile without debug options

## Install

I'm sorry but there is no implementation of ```make install``` command for this process!

So, just copy one of the following plugin binary
> ```sqlite_auth_plugin.so``` 
>
>```sqlite_auth_plugin_debug.so``` 

to your mosquitto config directory, in most of cases, copy to 
>```/etc/mosquitto/```

And do not forget to copy the ```sqlite_auth_plugin_debug.conf``` to the ```/etc/mosquitto/conf.d``` too.

### If you are runnging in debug mode
Just execute the following command to test your plugin implamentantion
```# mosquitto -c /etc/mosquitto/mosquitto.conf```

# Already working!
- [ ] Simple authentication username/password
- [ ] IoT device group control
- [ ] ACL for topic control
- [ ] ACL for callback topic messages
- [ ] ACL for auto-registration methods
- [ ] Policy control to access topics