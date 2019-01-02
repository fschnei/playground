#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("example char device");
MODULE_LICENSE("GPL");

static int Major;
dev_t dev_no;

static int hello_init(void)
{
	int ret;
	printk(KERN_ALERT "hello_init function\n");
	printk (" Inside init module\n");
	ret = alloc_chrdev_region( &dev_no , 0, 1,"chr_arr_dev");
	if (ret < 0)
	{
		printk("Major number allocation is failed\n");
		return ret;
	}
	
	Major = MAJOR(dev_no);
	printk (" The major number of device is %d\n", Major);
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
