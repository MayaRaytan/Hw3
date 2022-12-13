#include "mesage_slot.h"
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#define STDOUT_FILENO 1

int perror_exit_1();

int perror_exit_1() {
    perror("");
    exit(1);
}

int main(int argc, char *argv[]) {
    char *message_file_path;
    unsigned int target_message_channel_id;
    int fd;
    char *buffer;

    if (argc == 3) {
        message_file_path = argv[1];
        target_message_channel_id = atoi(argv[2]);
        size_t message_len;
        /* open file */
        if ((fd = open(message_file_path, O_RDONLY, 0777)) == -1) {
            perror_exit_1();
        }

        /* set channel id*/
        if (ioctl(fd, MSG_SLOT_CHANNEL, target_message_channel_id) == -1){
            perror_exit_1();
        }

        /* read message from channel to user buffer */
        buffer = (char*)kmalloc(sizeof(char)*message_len, GFP_KERNEL);
        if ((message_len = read(fd, buffer, 128)) <= 0){
            perror_exit_1();
        }
        close(fd);

        /* write message from user buffer to stdout */
        if (write(STDOUT_FILENO, buffer, message_len) != message_len){
            perror_exit_1();
        }

        free(buffer);
        exit(0);
    }

    else{
        perror_exit_1();
    }
}