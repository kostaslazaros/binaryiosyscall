/**********************************************************
*           BINARY FILE I/O OPERATIONS                    *
*           --------------------------                    *
*  Developers: Nefeli Stefanatou & Konstantinos Lazaros   *
*  Lesson    : Advanced Programming                       *
***********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define DATAFILE "data.bin"
#define POSFILE "pos.bin"
#define BSIZE 128


int new_entry(){
    char entry[BSIZE];
    printf("Enter the new entry here: ");
    scanf(" %[^\n]", entry);
    int entry_size = strlen(entry);
    int fd1 = open(DATAFILE, O_WRONLY | O_CREAT, 0644);
    int nfile = open(POSFILE, O_WRONLY | O_CREAT, 0644);
    if (fd1 == -1){
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


void delete(){



}


void display_all(){
    if(access(DATAFILE, F_OK) == -1){
        printf("File does not exist. Returning\n");
        return;
    }
    char buffer[BSIZE];
    int anum;
    int tanum = 0;
    int fd1 = open(DATAFILE, O_RDONLY); //file descriptor that shows that file opened
    int nfile = open(POSFILE, O_RDONLY);
    while(read(nfile, &anum, sizeof(int))){ //read the file that file decriptor shows,read sizeof(int) bytes a time and put them into the adress of anum//
        lseek(nfile, 0, SEEK_CUR); //leaves us at the position we were the last time we read
        printf("%i: ", anum); //print the byte numbers
        memset(buffer, '\0', BSIZE*sizeof(char));
        tanum += anum;//total bytes read
        read(fd1, buffer, anum);//read the file that fd1 describes, read anum bytes a time and put them in buffer
        printf("%s\n", buffer);//print the bytes that are in the buffer
        lseek(fd1, tanum, SEEK_SET);//starts from the start (SEEK_SET) t and goes to tantum
    }
    close(nfile);
    close(fd1);
}


void find(){
    if(access(DATAFILE, F_OK) == -1){
        printf("File does not exist. Returning\n");
        return;
    }
    char buffercomp[BSIZE];
    printf("Enter the entry you want to find: ");
    scanf(" %s", buffercomp);
    char buffer[BSIZE];
    int anum;
    int tanum = 0;
    int fd1 = open(DATAFILE, O_RDONLY); //file descriptor that shows that file opened
    int nfile = open(POSFILE, O_RDONLY);
    int found = 0;
    while(read(nfile, &anum, sizeof(int))){ //read the file that file decriptor shows,read sizeof(int) bytes a time and put them into the adress of anum//
        lseek(nfile, 0, SEEK_CUR); //leaves us at he position we were the last time we read
        // printf("%i: ", anum); //print the byte numbers
        tanum += anum;//total bytes read
        memset(buffer, '\0', BSIZE*sizeof(char));
        read(fd1, buffer, anum);//read the file that fd1 describes, read anum bytes a time and put them in buffer
        if(strncmp(buffer, buffercomp, strlen(buffercomp)) == 0){
            printf("Found entry: %s\n", buffer);
            found = 1;
            break;
        }
        lseek(fd1, tanum, SEEK_SET);//starts from the start (SEEK_SET) t and goes to tantum
    }
    close(nfile);
    close(fd1);
    if(!found){
        printf("Entry not found.\n");
    }
}


int main(){
    int choice = 0;
    while(choice != 5){
        //system("@cls||clear");
        //printf("Selection menu: \n");
        printf("(1) New ");
        printf("(2) Find ");
        printf("(3) Delete ");
        printf("(4) Display all ");
        printf("(5) Exit ");
        printf("Choose: ");
        scanf("%i", &choice);
        switch (choice){
            case 1:
                new_entry();
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
            default:
                break;
        }
    }
    return 0;
}
