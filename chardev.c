#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Viktor V.");

static int major_number;
static char msg[BUFFER_SIZE] = {0};

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
    if (*offset >= BUFFER_SIZE)
        return 0;

    if (*offset + length > BUFFER_SIZE)
        length = BUFFER_SIZE - *offset;

    if (copy_to_user(buffer, msg + *offset, length) != 0)
        return -EFAULT;

    *offset += length;
    return length;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
    if (length > BUFFER_SIZE)
        return -EINVAL;

    if (copy_from_user(msg, buffer, length) != 0)
        return -EFAULT;

    return length;
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int __init chardev_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }

    printk(KERN_INFO "Registered char device with major number %d\n", major_number);
    return 0;
}

static void __exit chardev_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Unregistered char device\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

