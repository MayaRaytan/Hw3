#include "mesage_slot.h"
#include <stdlib.h>
#include <string.h>

#define STDOUT_FILENO 1

int perror_exit_1();

int perror_exit_1() {
    perror("");
    exit(1);
}

int main(int argc, char *argv[]) {
    char *message_file_path;
    unsigned long target_message_channel_id;
    int fd;
    char *buffer;
    int message_len;

    if (argv == 3) {
        message_file_path = argv[1];
        target_message_channel_id = atoi(argv[2]);
        char* message;
        size_t message_len;
        if ((fd = open(message_file_path, O_RDONLY, 0777)) == -1) {
            perror_exit_1();
        }

        if (device_ioctl(fd, MSG_SLOT_CHANNEL, target_message_channel_id) == -1){
            perror_exit_1();
        }

        if (fd->private_data->channel-channel != target_message_channel_id){
            perror_exit_1();
        }

        message = fd->private_data->channel->message;
        message_len = strlen(message);

        buffer = (struct char*)kmalloc(sizeof(char)*message_len, GFP_KERNEL);
        if (read(fd, buffer, message_len) != message_len){
            perror_exit_1();
        }

        close(fd);

        if (write(STDOUT_FILENO, buffer, message_len) != message_len){
            perror_exit_1();
        }

        exit(0);
    }

    else{
        perror_exit_1();
    }
}