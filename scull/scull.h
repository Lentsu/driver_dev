#ifndef GUARD_SCULL_H
#define GUARD_SCULL_H

#define SCULL_MAJOR 0
#define SCULL_NR_DEVS 4

#define SCULL_NAME "scull"

static int __init scull_init(void);
static void __exit scull_exit(void);

struct scull_dev {

	struct scull_qset *data;	/*	POINTER TO THE FIRST QUANTUM SET	*/
	int quantum;				/*	THE CURRENT QUANTUM SIZE 			*/
	int qset;					/*	THE CURRENT ARRAY SIZE 				*/
	unsigned long size;			/*	AMOUNT OF DATA STORED HERE			*/
	unsigned int access_key;	/*	USED BY SCULLUID AND SCULLPRIV		*/
	struct semaphore sem;		/*	MUTUAL EXCLUSION SEMAPHORE			*/
	struct cdev cdev;			/*	CHARACTER DEVICE					*/

}; 

// A method to setup scull character device structure
static void scull_setup_cdev(struct scull_dev*, int);


#endif
