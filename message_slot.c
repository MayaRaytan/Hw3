#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE

#include <linux/slab.h>
#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/fs.h>       /* for register_chrdev */
#include <linux/uaccess.h>  /* for get_user and put_user */
#include <linux/string.h>   /* for memset. NOTE - not string.h!*/
#include "mesage_slot.h"

MODULE_LICENSE("GPL");

struct file_descriptor {
    struct device_channels* channel;
    int minor;
};

struct device_channels {
    char* message;
    unsigned long channel;
    struct device_channels* next;
};

struct device_channels last_channel(struct device_channels* channel) {
    struct device_channels* head;
    head = channel;
    while (head->next){
        head = head->next;
    }
    return head;
}

/* file-process, inode-device */
static int device_open(struct file* file, struct inode* inode) {
    int idx, minor;
    unsigned long flags;
    struct device_channels* device_channel;
    struct file_descriptor file_des*;

    minor = iminor(inode);

    file_des = (struct file_descriptor*)kmalloc(sizeof(file_descriptor), GFP_KERNEL);
    if (file_des == NULL){
        return 0;
    }
    file_des->minor = minor;
    file->private_data=file_des;
    /* crete a new device */

    if (device_files[minor] == NULL) {
        device_channel = (struct device_channels*) kmalloc(sizeof(struct device_channels), GFP_KERNEL);
        if (device_channel == NULL) {
            return 0;
        }
        device_files[minor] = device_channel;
    }
    return 1;
}


static long device_ioctl(struct file* file, unsigned int ioctl_command_id, unsigned long ioctl_param){
    int minor;
    struct device_channels* last_device_channel;
    struct device_channels* new_device_channel;

    if (ioctl_command_id != MSG_SLOT_CHANNEL || ioctl_param == 0){
        return -EINVAL;
    }
    file->private_data->file_des = ioctl_param;
    minor = file->private_data->minor;

    last_device_channel = last_channel(device_files[minor]);

    new_device_channel = (struct device_channels*) kmalloc(sizeof(struct device_channels), GFP_KERNEL);
    if (new_device_channel == NULL){
        return 0;
    }

    new_device_channel->channel = ioctl_param;
    last_device_channel->next = new_device_channel;

    file->file_descriptor->channel = new_device_channel;

    /* ????????????????????????? */
    return ioctl_param;
}

static ssize_t device_read(struct file* file, char __user* buffer, size_t length, loff_t* offset){
    int i;
    unsigned long channel;
    char* message;

    channel = file->file_descriptor->channel->channel;

    if (channel == NULL){
        return -EINVAL;
    }

    message = channel->message;

    if (message == NULL){
        return -EWOULDBLOCK;
    }

    if (length < strlen(message)){
        return -ENOSPC;
    }

    for (i = 0; i < strlen(message); i++) {
        put_user(message[i], &buffer[i]);
    }

    return 1;
}

static ssize_t device_write(struct file* file, const char __user* buffer, size_t length, loff_t* offset){

}

/* rec06 */
struct file_operations Fops = {
        .owner	  = THIS_MODULE,
        .read           = device_read,
        .write          = device_write,
        .open           = device_open,
        .unlocked_ioctl = device_ioctl,
        .release        = device_release,
};

static int __init simple_init(void) {
    int rc;
    memset(&device_info, 0, sizeof(struct chardev_info));
    rc = register_chrdev(MAJOR_NUMBER, DEVICE_RANGE_NAME, &Fops);

    if( rc < 0 ) {
        return rc;
    }

    return 0;
}

static void __exit simple_cleanup(void) {
    unregister_chrdev(MAJOR_NUMBER, DEVICE_RANGE_NAME);
}


module_init(simple_init);
module_exit(simple_cleanup);



