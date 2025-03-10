#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>

#define DEMO_NAME "my_demo_dev_misc"
#define MIN(a,b) ((a)<(b) ? (a):(b))

static struct device *mydemodrv_device;
static char kernel_buf[1024];

static int demodrv_open(struct inode *inode, struct file *file)
{
    int major = MAJOR(inode->i_rdev);
    int minor = MINOR(inode->i_rdev);

    printk("%s:major = %d,minor = %d\n",__func__,major,minor);

    return 0;
}

//int (*release) (struct inode *, struct file *);
static int demodrv_release(struct inode *inode, struct file *file)
{
    return 0;
}

//	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);

static ssize_t
demodrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
    printk("%s enter\n",__func__);
    copy_to_user(buf,kernel_buf,MIN(1024,lbuf));
    return MIN(1024,lbuf);
}
//	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
static ssize_t
demodrv_write(struct file *file, const char __user *buf, size_t count, loff_t *f_ops)
{
    printk("%s enter\n",__func__);
    copy_from_user(kernel_buf,buf,MIN(1024,count));
    return MIN(1024,count);
}


static const struct file_operations demodrv_fops = {
    .owner = THIS_MODULE,
    .open = demodrv_open,
    .release = demodrv_release,
    .read = demodrv_read,
    .write = demodrv_write
};

static struct miscdevice mydemodrv_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEMO_NAME,
    .fops = &demodrv_fops,
};

static int __init simple_char_init(void)
{
    int ret;
    //ret = alloc_chrdev_region(&dev,0,count,DEMO_NAME);
    ret = misc_register(&mydemodrv_misc_device);
    if(ret) {
        printk("failed register misc device\n");
        return ret;
    }

    mydemodrv_device = mydemodrv_misc_device.this_device;

    printk("succeeded register misc device: %s\n",DEMO_NAME);

    return 0;
}

static void __exit simple_char_exit(void)
{
    printk("removing device\n");

    misc_deregister(&mydemodrv_misc_device);
}

module_init(simple_char_init);
module_exit(simple_char_exit);

MODULE_AUTHOR("silence");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("simple character device");