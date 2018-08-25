#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm/uaccess.h>

#define DRIVER_NAME "mydevice"
#define DRIVER_NAJOR 63

static int mydevice_open(struct inode *inode, struct file *file)
{
	printk("mydevice open\n");
	return 0;
}

static int mydevice_close(struct inode *inode, struct file *file)
{
	printk("mydevice close\n");
	return 0;
}

static ssize_t mydevice_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("mydevice read");
	buf[0] = 'A';
	return 1;
}

static ssize_t mydevice_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	printk("mydevice write");
	return 1;
}

struct file_operations s_mydevice_fops = {
	.open = mydevice_open,
	.release = mydevice_close,
	.read = mydevice_read,
	.write = mydevice_write,
};

static int mydevice_init(void)
{
    printk("mydevice init\n");
	register_chrdev(DRIVER_NAJOR, DRIVER_NAME, &s_mydevice_fops);
    return 0;
}

static void mydevice_exit(void)
{
    printk("mydevice exit\n");
	unregister_chrdev(DRIVER_NAJOR, DRIVER_NAME);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
