/*
 *	LINUX MODULE INCLUDES
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>	// For dev_t
#include <linux/fs.h>		// For dev_t methods and fops
#include <linux/cdev.h>		// For the CDEV structure

/*
 *	LOCAL INCLUDES
 */ 
#include "scull.h"
	
/*	GLOBALS	*/

int scull_major = SCULL_MAJOR;
int scull_minor = 0;
dev_t devn;
int scull_nr_devs = SCULL_NR_DEVS;
const char* scull_name = SCULL_NAME;

/*	SCULL DEVICE STRUCTURE	*/
scull_dev* s_dev;
struct file_operations scull_fops;	// fops for scull device

/*	END OF GLOBALS	*/

/*
 * MODULE'S INIT FUNCTION
 */
static int __init scull_init(void) {
	
	/*
	 *	ALLOCATE DEVICE NUMBERS	
	 */
	
	printk(KERN_ALERT "scull: Allocating device numbers.\n", scull_major);
	
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
	
	//	Check that all operations were successful
	scull_major = MAJOR(devn);	// Get major
	if (reg_chrdev_res) {
		printk(KERN_ALERT "scull: can't get major %d\n", scull_major);
		return reg_chrdev_res;
	}

	// allocate cdev structure inside s_dev
	s_dev->cdev = cdev_alloc();
	
	// setup the scull device structure
	scull_setup_cdev(s_dev, 0);

	// RETURN WITH NO INIT ERRORS
	return 0;
}

scull_setup_cdev(struct scull_dev* dev, int index) {
	
	int err;
	int devno = MKDEV(scull_major, scull_minor + index);

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.fops = &scull_fops;

	// Try adding the character device
	err = cdev_add(&dev->cdev, devno, 1);
	// Fail gracefully if needed
	if (err)
		printk(KERN_NOTICE "Error: %d adding scull%d", err, index);
}

/*
 * MODULE'S EXIT FUNCTION
 *
 * USED TO FREE EVERYTHING IN REVERSE ORDER.
 *
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
