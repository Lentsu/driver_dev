KVER = $(shell uname -r)

# If KERNELRELEASE is defined, we are in the
# Kernel build system and can use it's language
ifneq ($(KERNELRELEASE),)
	obj-m := scull.o
	# module-objs :=

# Otherwise we were called directly from command line
# Change the build environment
else
	KDIR	?= /lib/modules/$(KVER)/build
	PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.ko *.o *.mod* modules* Module* .*.cmd

endif
