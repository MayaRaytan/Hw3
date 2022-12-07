#include "mesage_slot.h"
#include <stdlib.h>
#include <string.h>

int perror_exit_1();

int perror_exit_1(){
    perror("");
    exit(1);
}

int main(int argc, char *argv[]) {
    char *message_file_path;
    unsigned long target_message_channel_id;
    char *message_to_pass;
    device *d;
    int idx;
    int fd;
    int bytes;

    if (argv == 4) {
        message_file_path = argv[1];
        target_message_channel_id = atoi(argv[2]);
        message_to_pass = argv[3];
        if ((fd = open(file, O_WRONLY, 0777)) == -1) {
            perror_exit_1();
        }

        /* update channel ID */
        if (device_ioctl(fd, MSG_SLOT_CHANNEL, target_message_channel_id) == -1){
            perror_exit_1();
        }

        if (write(fd, message_to_pass, strlen(message_to_pass)) != strlen(message_to_pass)){
            perror_exit_1();
        }
        close(fd);
        exit(0);
    }

    else{
        perror_exit_1();
    }

}