cmd_/home/lentsu/driver_dev/hello/Module.symvers := sed 's/ko$$/o/' /home/lentsu/driver_dev/hello/modules.order | scripts/mod/modpost -m   -o /home/lentsu/driver_dev/hello/Module.symvers -e -i Module.symvers   -T -
