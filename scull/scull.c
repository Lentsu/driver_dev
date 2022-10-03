/*
 *	LINUX MODULE INCLUDES
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>	// For dev_t
#include <linux/fs.h>		// For device number registeration 

/*
 *	LOCAL INCLUDES
 */ 
#include "scull.h"
	
/*
 *	GLOBALS
*/
int scull_major = SCULL_MAJOR;
int scull_minor = 0;
dev_t dev;
int scull_nr_devs = SCULL_NR_DEVS;
const char* scull_name = SCULL_NAME;

static int __init scull_init(void) {
	
	printk(KERN_ALERT "scull: Allocating device numbers.\n", scull_major);
	/*
	 *	ALLOCATE DEVICE NUMBERS	
	 */
	
	// Initialize device number struct
	dev = 0;

	int reg_chrdev_res;	// Holds the return value of device number registeration

	// If major not set, allocate both numbers
	if ( scull_major ) {
		dev = MKDEV( scull_major, scull_minor );
		reg_chrdev_res = register_chrdev_region( dev, scull_nr_devs, scull_name);
	} else {
		reg_chrdev_res = alloc_chrdev_region( &dev, scull_minor,
											scull_nr_devs, scull_name );
	}
	
	// Get major
	scull_major = MAJOR(dev);
	
	/*	
	 *	CHECK THAT ALL OPERATIONS WERE SUCCESSFULL	
	 */
	if (reg_chrdev_res) {
		printk(KERN_ALERT "scull: can't get major %d\n", scull_major);
		return reg_chrdev_res;
	}

	return 0;
}

/* 
 * FUNCTION TO RETURNED THE USED RESOURCES.
 * 	(UNLOADS EVERYTHING IN REVERSE ORDER)
 */
static void __exit scull_exit(void) {
	// unregister device numbers <
	printk(KERN_ALERT "scull: trying to unregister chrdev '%s' with major %d\n",
						scull_name, scull_major);
	unregister_chrdev( scull_major, scull_name );
	// >
}

// Init and exit as declared in linux/init.h
module_init(scull_init);
module_exit(scull_exit);

// License section
MODULE_LICENSE("GPLv2.0");
MODULE_AUTHOR("Lenni Toikkanen");
