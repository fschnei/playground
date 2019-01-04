#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");

static int sample_cdev_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "open sample char device\n");
    return 0;
}

static int sample_cdev_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "release sample char device\n");
    return 0;
}

static ssize_t sample_cdev_read(struct file *file, char __user *data, size_t size, loff_t *offset) {
    return size;
}

static ssize_t sample_cdev_write(struct file *file, const char __user *data, size_t size, loff_t *offset) {
    return size;
}


static struct file_operations sample_cdev_fops = {
    .owner = THIS_MODULE,
    .open = sample_cdev_open,
    .release = sample_cdev_release,
    .read = sample_cdev_read,
    .write = sample_cdev_write,
};

static dev_t sample_dev_t = 0;
static struct class* sample_class;
static struct cdev* sample_cdev;

static int __init sample_cdev_init(void) {

    int result;
    const int firstMinorNumber = 5;
    const int minorNumberCount = 1;

    printk(KERN_DEBUG "sample char device init\n");

    printk ("minorNumberCount is %d\n", minorNumberCount);

    // get major and minor numbers dynamically
    // sample_dev_t is a number which holds the major and the first minor number (32 Bit value)
    result = alloc_chrdev_region(&sample_dev_t, firstMinorNumber, minorNumberCount, "sampleCharDeviceName");
    if(result < 0) {
	printk(KERN_ERR "failed to alloc chrdev region\n");
	goto fail_alloc_chrdev_region;
    }
    printk ("The sample_dev_t number for your device is %d\n", sample_dev_t);
    printk ("The major number for your device is %d\n", MAJOR(sample_dev_t));
    printk ("The minor number for your device is %d\n", MINOR(sample_dev_t));

    // cdev is a structure (defined in linux/cdev.h) which represent the device with read, write, ... function pointers
    sample_cdev = cdev_alloc();
    if(!sample_cdev) {
	result = -ENOMEM;
	printk(KERN_ERR "failed to alloc cdev\n");
	goto fail_alloc_cdev;
    }
    // now the cdev struct is undefined -> init it

    // the following has to be done for every cdev device (every minor number)
    // init my cdev object (also add pointer to my fileOperationsStruct)
    cdev_init(sample_cdev, &sample_cdev_fops);
    // concat the collected major number and minor numbers (eventually with for loop) with the allocated character device
    result = cdev_add(sample_cdev, MKDEV(MAJOR(sample_dev_t), MINOR(sample_dev_t)), 1);
    if(result < 0) {
	printk(KERN_ERR "failed to add cdev\n");
	goto fail_add_cdev;
    }

    // check if init was successfully
    printk ("sample_cdev->count is %d\n", sample_cdev->count);
    printk ("sample_cdev->dev is %d\n", sample_cdev->dev);
    printk ("sample_cdev->dev is %d\n", sample_cdev->dev);

    // create virtual device
    sample_class = class_create(THIS_MODULE, "sample");
    if(!sample_class) {
	result = -EEXIST;
	printk(KERN_ERR "failed to create class\n");
	goto fail_create_class;
    }
    // create a device in /dev and register the virtual device from class_create
    if(!device_create(sample_class, NULL, sample_dev_t, NULL, "sample_cdev%d", MINOR(sample_dev_t))) {
	result = -EINVAL;
	printk(KERN_ERR "failed to create device\n");
	goto fail_create_device;
    }
    return 0;
fail_create_device:
    class_destroy(sample_class);
fail_create_class:
    cdev_del(sample_cdev);
fail_add_cdev:
fail_alloc_cdev:
    unregister_chrdev_region(sample_dev_t, 1);
fail_alloc_chrdev_region:
    return result;
}

static void __exit sample_cdev_exit(void) {
    printk(KERN_DEBUG "sample char device exit\n");
    device_destroy(sample_class, sample_dev_t);
    class_destroy(sample_class);
    cdev_del(sample_cdev);
    unregister_chrdev_region(sample_dev_t, 1);
}

module_init(sample_cdev_init);
module_exit(sample_cdev_exit);
