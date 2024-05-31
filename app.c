#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_FILE "/dev/chardev"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char buffer[BUFFER_SIZE] = "Hello from userspace!";

    fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    write(fd, buffer, BUFFER_SIZE);

    read(fd, buffer, BUFFER_SIZE);
    printf("Message from kernel: %s\n", buffer);

    close(fd);

    return 0;
}

