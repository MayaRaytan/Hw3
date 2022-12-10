#include "mesage_slot.h"
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

int perror_exit_1();

int perror_exit_1(){
    perror("");
    exit(1);
}

int main(int argc, char *argv[]) {
    char *message_file_path;
    unsigned int target_message_channel_id;
    char *message_to_pass;
    int fd;

    if (argc == 4) {
        message_file_path = argv[1];
        target_message_channel_id = atoi(argv[2]);
        message_to_pass = argv[3];
        /* open file */
        if ((fd = open(message_file_path, O_WRONLY, 0777)) == -1) {
            perror_exit_1();
        }

        /* update channel ID */
        if (ioctl(fd, MSG_SLOT_CHANNEL, target_message_channel_id) == -1){
            perror_exit_1();
        }

        /* write message to channel */
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