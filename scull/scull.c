/*
 *	LINUX MODULE INCLUDES
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>	// For dev_t
#include <linux/fs.h>		// For device number registeration
#include <linux/cdev.h>		// For

/*
 *	LOCAL INCLUDES
 */ 
#include "scull.h"
	
/*
 *	GLOBALS
*/
int scull_major = SCULL_MAJOR;
int scull_minor = 0;
dev_t devn;
int scull_nr_devs = SCULL_NR_DEVS;
const char* scull_name = SCULL_NAME;

// Scull device structure
scull_dev* s_dev;

static int __init scull_init(void) {
	
	printk(KERN_ALERT "scull: Allocating device numbers.\n", scull_major);
	/*
	 *	ALLOCATE DEVICE NUMBERS	
	 */
	
	// Initialize device number struct
	devn = 0;

	int reg_chrdev_res;	// Holds the return value of device number registeration

	// If major not set, allocate both numbers
	if ( scull_major ) {
		devn = MKDEV( scull_major, scull_minor );
		reg_chrdev_res = register_chrdev_region( devn, scull_nr_devs, scull_name);
	} else {
		reg_chrdev_res = alloc_chrdev_region( &devn, scull_minor,
											scull_nr_devs, scull_name );
	}
	
	/*	
	 *	CHECK THAT ALL OPERATIONS WERE SUCCESSFULL	
	 */
	scull_major = MAJOR(devn);	// Get major
	if (reg_chrdev_res) {
		printk(KERN_ALERT "scull: can't get major %d\n", scull_major);
		return reg_chrdev_res;
	}

	/*
	 * ALLOCATE CDEV STRUCTURE
	 */
	struct cdev* s_dev->cdev = cdev_alloc();
	
	/*
	 * SETUP THE SCULL DEVICE STRUCTURE
	 */
	scull_setup_cdev();

	/*
	 * NO INIT ERRORS
	 */
	return 0;
}

/* 
 * FUNCTION TO RETURN THE USED RESOURCES.
 * 	(UNLOADS EVERYTHING IN REVERSE ORDER)
 */

static void __exit scull_exit(void) {
	
	// unregister character device structure <
	cdev_del(&scull_dev->cdev);
	// >

	// unregister device numbers <
	printk(KERN_ALERT "scull: trying to unregister chrdev '%s' with major %d\n",
						scull_name, scull_major);
	unregister_chrdev( scull_major, scull_name );
	// >
}

/*
 * SCULL FILE OPERATIONS STRUCTURE (DECLARED IN HEADER)
 */
scull_fops = {

	.owner		= THIS_MODULE,
	.llseek		= scull_llseek,
	.read		= scull_read,
	.write		= scull_write,
	.ioctl		= scull_ioctl,
	.open		= scull_open,
	.release	= scull_release
};

// Init and exit as declared in linux/init.h
module_init(scull_init);
module_exit(scull_exit);

// License section
MODULE_LICENSE("GPLv2.0");
MODULE_AUTHOR("Lenni Toikkanen");
