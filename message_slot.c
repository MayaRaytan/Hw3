#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE

/* code from recitation */
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

struct device_channels* get_channel(struct device_channels* head, unsigned int channel_id){
    struct device_channels* channel_node;
    channel_node = head;
    while (channel_node != NULL){
        if (channel_node->channel == channel_id){
            return channel_node;
        }
    }
    return NULL;
}


static int device_open(struct file* file, struct inode* inode) {
    int idx, minor;
    struct device_channels* device_channel;
    struct file_descriptor file_des*;

    minor = iminor(inode);

    file_des = (struct file_descriptor*)kmalloc(sizeof(file_descriptor), GFP_KERNEL);
    if (file_des == NULL){
        return -ENOMEM;
    }
    file_des->minor = minor;
    file->private_data=file_des;

    /* crete a new device */
    if (device_files[minor] == NULL) {
        device_channel = (struct device_channels*) kmalloc(sizeof(struct device_channels), GFP_KERNEL);
        if (device_channel == NULL) {
            return -ENOMEM;
        }
        device_files[minor] = device_channel;
    }
    return 0;
}


static long device_ioctl(struct file* file, unsigned int ioctl_command_id, unsigned int ioctl_param){
    int minor;
    struct device_channels* exist_channel;
    struct device_channels* last_device_channel;
    struct device_channels* new_device_channel;

    if (ioctl_command_id != MSG_SLOT_CHANNEL || ioctl_param == 0) {
        return -EINVAL;
    }

    minor = file->private_data->minor;

    /* check */
    if (device_files[minor] == NULL){
        return -1
    }

    exist_channel = get_channel(device_files[minor], channel_id);
    if (exist_channel == NULL){
        last_device_channel = last_channel(device_files[minor]);

        new_device_channel = (struct device_channels*) kmalloc(sizeof(struct device_channels), GFP_KERNEL);
        if (new_device_channel == NULL){
            return 0;
        }

        new_device_channel->channel = ioctl_param;
        last_device_channel->next = new_device_channel;
        exist_channel = new_device_channel;
    }

    file->private_data->channel = new_device_channel;

    /* ????????????????????????? */
    return ioctl_param;
}

static ssize_t device_read(struct file* file, char __user* buffer, size_t length){
    ssize_t i;
    unsigned int channel;
    char* message;

    channel = file->private_data->channel->channel;
    if (channel == NULL){
        return -EINVAL;
    }

    message = file->private_data->channel->message;
    if (message == NULL){
        return -EWOULDBLOCK;
    }

    if (length < strlen(message)){
        return -ENOSPC;
    }

    for (i = 0; i < strlen(message); i++) {
        put_user(message[i], &buffer[i]);
    }

    return i;
}

static ssize_t device_write(struct file* file, const char __user* buffer, size_t length) {
    ssize_t i;
    unsigned long channel;
    char* message;

    channel = file->private_data->channel->channel;
    if (channel == NULL){
        return -EINVAL;
    }

    if (length > 128 || length < 0){
        return -EMSGSIZE;
    }

    message = (char*)kmalloc(sizeof(char)*length), GFP_KERNEL);
    if (message == NULL){
        return -ENOMEM;
    }

    for (i = 0; i < strlen(message); i++) {
        get_user(message[i], &buffer[i]);
    }

    file->private_data->channel->message = message;

    return i;
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

void free_file(struct file* file){
    kfree(file->private_data);
}

static int device_release(struct inode* inode, struct file* file) {
    free_file(file);
    return 0;
}

void free_devices(){
    /* iterate over device_files and free all channels */
    for (i = 0; i < 256; i++){
        head = device_files[i];
        prev = head
        while (head != NULL){
            head = head->next;
            kfree(prev);
            prev = head;
        }
        kfree(device_files[i])
    }
}

static int __init simple_init(void) {
    int rc;
//    memset(&device_info, 0, sizeof(struct chardev_info));
    rc = register_chrdev(MAJOR_NUMBER, "Message_slot", &Fops);
    if( rc < 0 ) {
        return rc;
    }
    device_files = (struct device_channels*)kmalloc(256*sizeof(struct device_channels), GFP_KERNEL);
    if (device_files == NULL){
        return -ENOMEM;
    }
    return 0;
}

static void __exit simple_cleanup(void) {
    int i;
    struct device_channels* head, prev;
    unregister_chrdev(MAJOR_NUMBER, "Message_slot");
    free_devices();
}


module_init(simple_init);
module_exit(simple_cleanup);



