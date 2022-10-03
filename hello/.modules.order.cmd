cmd_/home/lentsu/driver_dev/hello/modules.order := {   echo /home/lentsu/driver_dev/hello/hello.ko; :; } | awk '!x[$$0]++' - > /home/lentsu/driver_dev/hello/modules.order
