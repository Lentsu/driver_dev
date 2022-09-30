#include <linux/init.h>
#include <linux/module.h>
#include "hello.h"

static int __init hello_init(void) {
	return hello_fun();
}

static void hello_exit(void) {
	printk(KERN_ALERT "Goodbye.\n");
}

static int hello_fun(void) {
	printk(KERN_ALERT "Hello from the other siiiiiiiideeeee!\n");
	return 0;
}

// Init and exit as declared in linux/init.h
module_init(hello_init);
module_exit(hello_exit);

// License section
MODULE_LICENSE("GPL2.0");
MODULE_AUTHOR("Lenni Toikkanen");
