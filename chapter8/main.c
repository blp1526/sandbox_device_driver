#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>

MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "mydevice"

struct dentry *debug_entry_dir;
static int debug_prm1;
static int debug_read_size = 0;

static int mydevice_debug_open(struct inode *inode, struct file *file)
{
	printk("mydevice_debug_open\n");
	debug_read_size = 4;
	return 0;
}

static ssize_t mydevice_debug_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("mydevice_debug_read\n");

	if (debug_read_size > 0) {
		int len;
		len = sprintf(buf, "%d\n", debug_prm1);
		debug_read_size -= 4;
		return len;
	} else {
		return 0;
	}
}

static ssize_t mydevice_debug_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	printk("mydevice_debug_write\n");

	sscanf(buf, "%d", &debug_prm1);
	return count;
}

static struct file_operations debug_debug_prm1_fops = {
	.owner = THIS_MODULE,
	.open = mydevice_debug_open,
	.read = mydevice_debug_read,
	.write = mydevice_debug_write,
};

static int mydevice_init(void)
{
	printk("mydevice_init\n");

	debug_entry_dir = debugfs_create_dir(DRIVER_NAME, NULL);
	if (debug_entry_dir == NULL) {
		printk(KERN_ERR "debugfs_create_dir\n");
		return -ENOMEM;
	}

	debugfs_create_file("prm1", S_IRUGO | S_IWUGO, debug_entry_dir, NULL, &debug_debug_prm1_fops);
	return 0;
}

static void mydevice_exit(void)
{
	printk("mydevice_exit\n");

	debugfs_remove_recursive(debug_entry_dir);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
