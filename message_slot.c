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


struct device_file{
    struct file* file;
    struct inode* inode;
}device;

/* rec06 */
struct file_operations Fops = {
        .owner	  = THIS_MODULE,
        .read           = device_read,
        .write          = device_write,
        .open           = device_open,
        .unlocked_ioctl = device_ioctl,
        .release        = device_release,
};


device_files_len = 0;

int device_search(struct file* file) {
    for (i = 0; i < device_files_len; i++) {
        if (device_files[i].file == file) {
            return i;
        }
    }
    return -1;
}

static int device_open(struct file* file, struct inode* inode) {
    int idx;
    device *new_device_file;
    idx = device_search(file);
    /* crete a new device */
    if (idx == -1) {
        new_device_file = (device *) kmalloc(sizeof(device), GFP_KERNEL);
        if (new_device_file == NULL) {
            return 0;
        }
        new_device_file->file = file;
        new_device_file->inode = inode;
        device_files[device_files_len] = new_device_file;
        device_files_len++;
    }
    return 1;
}



//    int minor;
//    minor = iminor(file);


}

static long device_ioctl(struct file* file, unsigned int ioctl_command_id, unsigned long ioctl_param){
    if (ioctl_command_id != MSG_SLOT_CHANNEL){
        errno = EINVAL;
        return -1;
    }
    if (ioctl_param == 0){
        errno = EINVAL;
        return -1;
    }
    file->private_data = (long*)ioctl_param;
    /* ??????????????????????????????????????????????????????????????????????????? */
}

static ssize_t device_read(struct file* file, char __user* buffer, size_t length, loff_t* offset){
    int idx;
    unsigned long channel;
    idx = device_search(file);
    channel = ((device_files[idx])->file)->private_data;
    if (channel == Null){
        errno = EINVAL;
        return -1;
    }
    if channel

}

static ssize_t device_write(struct file* file, const char __user* buffer, size_t length, loff_t* offset){

}







