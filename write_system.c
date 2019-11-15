#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>





void function1(){

}


void function2(){

}

void function3(){

}

void function4(){

}












void funcsel()
{
    int choice;
    printf("Selection menu: \n");
    printf("1 is for adding entry\n");
    printf("2 is for seeking entry\n");
    printf("3 is for deleting entry\n");
    printf("4 is for displaying entry\n");
    printf("Select from 1-4 (or 5 to exit) : ");
    scanf("%d", choice );

    switch (choice)
    {
    case 1:
        function1();
        break;
    case 2:
        function2();
        break;
    case 3:
        function3();
        break;
    case 4:
        function4();
        break;
    case 5:
        exit(1);
    default:
        break;
    }

}


int main(int argc, char *argv[])
{
    funcsel();
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

    return 0;

}


