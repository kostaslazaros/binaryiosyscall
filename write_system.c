#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>



int main (int argc, char *argv[])
{
    int fd1;
    char buf[128];
    fd1 = open(argv[1], O_WRONLY | O_CREAT);
    if (fd1 == -1) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    /* Enter the data to be written into the file */
    scanf("%[^\n]s", buf);

    lseek(fd1, 12, SEEK_SET);

    write(fd1, buf, strlen(buf));

    close(fd1);

    printf("%i", strlen(buf));

    return 0;
}