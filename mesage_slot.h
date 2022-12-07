#define MAJOR_NUMBER = 235

int device_search(struct file* file);
static long device_ioctl(struct file* file, unsigned int ioctl_command_id, unsigned long ioctl_param);
device *device_files[256];
int device_files_len;