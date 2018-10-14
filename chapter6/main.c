#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "mydevice.h"

MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "mydevice"

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM = 1;

static unsigned int mydevice_major;
static struct cdev mydevice_cdev;
static struct class *mydevice_class = NULL;

static struct mydevice_values stored_values;

static long mydevice_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
	printk("mydevice_ioctl\n");

	switch (cmd) {
	case MYDEVICE_SET_VALUES:
		printk("MYDEVICE_SET_VALUES\n");
		if (raw_copy_from_user(&stored_values, (void __user *)arg, sizeof(stored_values))) {
			return -EFAULT;
		}
		break;

	case MYDEVICE_GET_VALUES:
		printk("MYDEVICE_GET_VALUES\n");
		if (raw_copy_to_user((void __user *)arg, &stored_values, sizeof(stored_values))) {
			return -EFAULT;
		}
		break;

	default:
		printk(KERN_WARNING "unsupported command %d\n", cmd);
		return -EFAULT;
	}

	return 0;
}

static int mydevice_open(struct inode *inode, struct file *file) {
	printk("mydevice_open\n");
	return 0;
}

static int mydevice_close(struct inode *inode, struct file *file) {
	printk("mydevice_close\n");
	return 0;
}

static ssize_t mydevice_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos) {
	printk("mydevice_read\n");
	return count;
}

static ssize_t mydevice_write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos) {
	printk("mydevice_write\n");
	return count;
}

static struct file_operations mydevice_fops = {
	.open = mydevice_open,
	.release = mydevice_close,
	.read = mydevice_read,
	.write = mydevice_write,
	.unlocked_ioctl = mydevice_ioctl,
	.compat_ioctl = mydevice_ioctl,
};

static int mydevice_init(void) {
	printk("mydevice_init\n");

	int alloc_ret = 0;
	int cdev_err = 0;
	dev_t dev;

	alloc_ret = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
	if (alloc_ret != 0) {
		printk(KERN_ERR "alloc_chrdev_region = %d\n", alloc_ret);
		return -1;
	}

	mydevice_major = MAJOR(dev);
	dev = MKDEV(mydevice_major, MINOR_BASE);

	cdev_init(&mydevice_cdev, &mydevice_fops);
	mydevice_cdev.owner = THIS_MODULE;

	cdev_err = cdev_add(&mydevice_cdev, dev, MINOR_NUM);

	if (cdev_err != 0) {
		printk(KERN_ERR "cdev_add = %d\n", alloc_ret);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	mydevice_class = class_create(THIS_MODULE, "mydevice");

	if (IS_ERR(mydevice_class)) {
		printk(KERN_ERR "class_create\n");
		cdev_del(&mydevice_cdev);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	for (int minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++) {
		device_create(mydevice_class, NULL, MKDEV(mydevice_major, minor), NULL, "mydevice%d", minor);
	}

	return 0;
}

static void mydevice_exit(void)
{
	printk("mydevice_exit\n");

	dev_t dev = MKDEV(mydevice_major, MINOR_BASE);

	for (int minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++) {
		device_destroy(mydevice_class, MKDEV(mydevice_major, minor));
	}

	class_destroy(mydevice_class);

	cdev_del(&mydevice_cdev);

	unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
