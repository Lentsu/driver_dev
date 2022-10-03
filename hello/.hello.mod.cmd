cmd_/home/lentsu/driver_dev/hello/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/lentsu/driver_dev/hello/"$$0) }' > /home/lentsu/driver_dev/hello/hello.mod
