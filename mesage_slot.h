#define MAJOR_NUMBER = 235
#define BUFFER_SIZE = 128

static long device_ioctl(struct file* file, unsigned int ioctl_command_id, unsigned long ioctl_param);

device_channels *device_files;
