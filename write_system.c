#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>


#define DATAFILE "data.bin"
#define POSFILE "pos.bin"
#define BSIZE 128


void fcopy()
{
    int arr[1];
    int data = open("start.txt", O_RDONLY);
    int copy = open("end.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    while (read(data, arr, 1))
    {

	printf("sdfsdf\n");
	lseek(data, 0, SEEK_CUR);
	write(copy, arr, 1);

    }

    close(data);
    close(copy);
}


int new_entry1(){
    char entry[BSIZE];
    printf("Enter the new entry here: ");
    scanf(" %[^\n]", entry);
    int entry_size = strlen(entry);
    int fd1 = open(DATAFILE, O_WRONLY | O_CREAT, 0644);
    int nfile = open(POSFILE, O_WRONLY | O_CREAT, 0644);
    if (fd1 == -1) {
        perror(DATAFILE);
        return EXIT_FAILURE;
    }
    /* Enter the data to be written into the file */
    lseek(fd1, 0, SEEK_END);
    lseek(nfile, 0, SEEK_END);
    write(fd1, entry, entry_size);
    write(nfile, &entry_size, sizeof(int));

    close(fd1);
    close(nfile);

    return 0;
}


void find(){
    char entry1[BSIZE];
    printf("Enter the entry you want to find: ");
    scanf(" %[^\n]s", entry1);
    printf("%s", entry1);
}


void delete(){



}


void display_all(){
    char buffer[BSIZE];
    int anum;
    int tanum = 0;
    int fd1 = open(DATAFILE, O_RDONLY); //file descriptor that shows that file opened
    int nfile = open(POSFILE, O_RDONLY);
    while(read(nfile, &anum, sizeof(int))){ //read the file that file decriptor shows,read sizeof(int) bytes a time and put them into the adress of anum//
        lseek(nfile, 0, SEEK_CUR); //leaves us at he position we were the last time we read
        printf("%i: ", anum); //print the byte numbers
        tanum += anum;//total bytes read
        read(fd1, buffer, anum);//read the file that fd1 describes, read anum bytes a time and put them in buffer
        printf("%s\n", buffer);//print the bytes that are in the buffer
        lseek(fd1, tanum, SEEK_SET);//starts from the start (SEEK_SET) t and goes to tantum
    }

    close(nfile);
    close(fd1);


}


void seek_and_retrieve(){


}


void funcsel(){

}


int main()
{
    int choice = 0;
    printf("Selection menu: \n");
    printf("1 New entry\n");
    printf("2 Find\n");
    printf("3 Delete\n");
    printf("4 Display all\n");
    printf("5 Exit\n");
    printf("Choose: ");
    scanf("%i", &choice);

    switch (choice)
    {
    case 1:
        new_entry1();
        break;
    case 2:
        find();
        break;
    case 3:
        delete();
        break;
    case 4:
        display_all();
        break;
    case 5:
        exit(1);
    default:
        break;
    }

/*    int fd1;
    char buf[128];
    fd1 = open(argv[1], O_WRONLY | O_CREAT);
    if (fd1 == -1) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }


    scanf("%[^\n]s", buf);

    lseek(fd1, 12, SEEK_SET);

    write(fd1, buf, strlen(buf));

    close(fd1);*/
    return 0;

}


