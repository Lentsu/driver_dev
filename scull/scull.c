/*
 *	LINUX MODULE INCLUDES
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	// For printk
#include <linux/types.h>	// For dev_t
#include <linux/fs.h>

/*
 *	LOCAL INCLUDES
 */ 
#include "scull.h"
	
/*
 *	GLOBALS
*/
int scull_major = SCULL_MAJOR;
int scull_minor = 0;
int scull_nr_devs = SCULL_NR_DEVS;
const char* scull_name = SCULL_NAME;

static int __init scull_init(void) {
	
	/*
	 *	ALLOCATE DEVICE NUMBERS	
	 */
	
	// Vars
	dev_t dev = 0;

	int reg_chrdev_res;	// Holds the return value of device number registeration

	// If major not set, allocate both numbers
	if ( scull_major ) {
		dev = MKDEV( scull_major, scull_minor );
		reg_chrdev_res = register_chrdev_region( dev, scull_nr_devs, scull_name);
	} else {
		reg_chrdev_res = alloc_chrdev_region( &dev, scull_minor,
											scull_nr_devs, scull_name );
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
	scull_clean();
}

/* 
 * FUNCTION TO RETURNED THE USED RESOURCES.
 * 	(UNLOADS EVERYTHING IN REVERSE ORDER)
 */
static void scull_clean() {
	// unregister device numbers <
		unregister_chrdev( scull_major, scull_name );
	// >
}

// Init and exit as declared in linux/init.h
module_init(scull_init);
module_exit(scull_exit);

// License section
MODULE_LICENSE("GPL2.0");
MODULE_AUTHOR("Lenni Toikkanen");
