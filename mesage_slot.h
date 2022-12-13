#include <errno.h>
#include <signal.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <stdio.h>
#define MAJOR_NUMBER = 235
#define BUFFER_SIZE = 128

static long device_ioctl(struct file* file, unsigned int ioctl_command_id, unsigned long ioctl_param);

struct device_channels *device_files;
