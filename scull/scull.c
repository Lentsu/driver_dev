#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>	// For dev_t

static int __init scull_init(void) {
	
	/*
	 * ALLOCATE DEVICE NUMBERS	
	 */
	
	// Vars
	int scull_major, scull_minor, scull_number_of_devs;
	scull_major = scull_minor = 0;
	scull_number_of_devs = 4;
	const char* scull_name = "scull";

	dev_t* dev;	
	int reg_chrdev_res;	// Holds the return value of device number registeration

	// If major not set, allocate both numbers
	if ( scull_major ) {
		dev = MKDEV( scull_major, scull_minor );
		reg_chrdev_res = register_chrdev_region(dev, scull_number_of_devs, scull_name);
	} else {
		reg_chrdev_res = alloc_chrdev_res( &dev, scull_minor,
											scull_number_of_devs, scull_name );
	}
	
	/*	
	 *	CHECK THAT ALL OPERATIONS WERE SUCCESSFULL	
	 */
	if (reg_chrdev_res) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		scull_clean();
	}

	return 0;
}

static void __exit scull_exit(void) {
	printk(KERN_INFO "scull: can't get major %d\n", scull_major);
	scull_clean();
}

/* 
 * FUNCTION TO RETURNED THE USED RESOURCES.
 * 	(UNLOADS EVERYTHING IN REVERSE ORDER)
 */
static void scull_clean() {
	// unregister device numbers <
		int unreg_chrdev_res;
		unreg_chrdev = unregister_chrdev( scull_major, scull_name );
	// >
}

// Init and exit as declared in linux/init.h
module_init(scull_init);
module_exit(scull_exit);

// License section
MODULE_LICENSE("GPL2.0");
MODULE_AUTHOR("Lenni Toikkanen");
