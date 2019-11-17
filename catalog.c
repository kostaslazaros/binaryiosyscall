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


int files_exist(){
    if((access(DATAFILE, F_OK) == -1) && (access(POSFILE, F_OK) == -1) ){
        printf("File does not exist. Returning\n");
        return 0;
    }
    return 1;
}


void load_data(char arr[][BSIZE]){
    if(! files_exist()) return;
    int anum2;
    int tanum = 0;
    int fd1 = open(DATAFILE, O_RDONLY);
    int nfile2 = open(POSFILE, O_RDONLY);
    int i = 0;
    while(read(nfile2, &anum2, sizeof(int))){
        lseek(nfile2, 0, SEEK_CUR);
        tanum += anum2;
        memset(arr[i], '\0', BSIZE*sizeof(char));
        read(fd1, arr[i], anum2);
        lseek(fd1, tanum, SEEK_SET);
        i++;
    }
    close(nfile2);
    close(fd1);
}

void save2file(char arr[][BSIZE], int arrsize){
    int srs = open(DATAFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int srn = open(POSFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    for (int j=0; j<arrsize; j++){
        long unsigned arlen = strlen(arr[j]);
        // printf("%i %s  : %lu\n", j, bfr[j], strlen(bfr[j]));
        lseek(srs, 0, SEEK_END);
        lseek(srn, 0, SEEK_END);
        write(srs, arr[j], arlen);
        write(srn, &arlen, sizeof(int));
    }
    close(srs);
    close(srn);
}


int get_number_of_records(){
    if(! files_exist()) return 0;
    int anum;
    int counter = 0;
    int nfile = open(POSFILE, O_RDONLY);
    while(read(nfile, &anum, sizeof(int))){
        counter++;
    }
    close(nfile);
    return counter;
}

static int cmp_str(const void *lhs, const void *rhs)
{
    return strcmp(lhs, rhs);
}


void sort_list(){
    if(! files_exist()) return;
    int counter = get_number_of_records();
    char bfr[counter][BSIZE];
    load_data(bfr);
    qsort(bfr, counter, sizeof bfr[0], cmp_str);
    save2file(bfr, counter);
}


int insert(){
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
    sort_list();
    return 0;
}


void delete(){
    if(! files_exist()) return;
    int counter = get_number_of_records();
    char bfr[counter][BSIZE];
    load_data(bfr);
    printf("\nList of Records\n");
    printf("---------------\n");
    for (int j=0; j<counter; j++){
        printf("%i.%s \n", j+1, bfr[j]);
    }
    int todelete;
    printf("\nSelect [record number] to delete or [0] to cancel: ");
    if (1 != scanf(" %d", &todelete)){
        printf("Invalid record number\n");
        return;
    };
    if(todelete > counter){
        printf("Number (%d) exceeds max record number (%d)\n", todelete, counter);
        return;
    }
    if(todelete == 0){
        printf("Delete cancelled\n");
        return;
    }
    int srs = open(DATAFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int srn = open(POSFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    for (int j=0; j<counter; j++){
        if (j == (todelete - 1))
            continue;
        long unsigned arlen = strlen(bfr[j]);
        // printf("%i %s  : %lu\n", j, bfr[j], strlen(bfr[j]));
        lseek(srs, 0, SEEK_END);
        lseek(srn, 0, SEEK_END);
        write(srs, bfr[j], arlen);
        write(srn, &arlen, sizeof(int));
    }
    close(srs);
    close(srn);
    printf("\nRecord No: %d deleted.\n", todelete);
}


void edit(){
    if(! files_exist()) return;
    int counter = get_number_of_records();
    char bfr[counter][BSIZE];
    load_data(bfr);
    printf("\nList of Records\n");
    printf("---------------\n");
    for (int j=0; j<counter; j++){
        printf("%i.%s \n", j+1, bfr[j]);
    }
    int toedit;
    printf("\nSelect [record number] to edit or [0] to cancel: ");
    if (1 != scanf(" %d", &toedit)){
        printf("Invalid record number\n");
        return;
    };
    if(toedit > counter){
        printf("Number (%d) exceeds max record number (%d)\n", toedit, counter);
        return;
    }

    if(toedit == 0){
        printf("Edit cancelled\n");
        return;
    }
    printf("Record to Edit: %s\n", bfr[toedit-1]);
    printf("New Value     : ");
    memset(bfr[toedit-1], '\0', BSIZE*sizeof(char));
    scanf(" %[^\n]", bfr[toedit-1]);
    qsort(bfr, counter, sizeof bfr[0], cmp_str);
    save2file(bfr, counter);
    printf("\nRecord No: %d updated.\n", toedit);
}


void display_all(){
    if(! files_exist()) return;
    char buffer[BSIZE];
    int anum;
    int tanum = 0;
    int fd1 = open(DATAFILE, O_RDONLY); //file descriptor that shows that file opened
    int nfile = open(POSFILE, O_RDONLY);
    int record_no = 1;
    printf("\nList Of Records\n");
    printf("---------------\n");
    while(read(nfile, &anum, sizeof(int))){ //read the file that file decriptor shows,read sizeof(int) bytes a time and put them into the adress of anum//
        lseek(nfile, 0, SEEK_CUR); //leaves us at the position we were the last time we read
        printf("%i.", record_no++);
        memset(buffer, '\0', BSIZE*sizeof(char));
        tanum += anum;//total bytes read
        read(fd1, buffer, anum);//read the file that fd1 describes, read anum bytes a time and put them in buffer
        printf("%s\n", buffer);//print the bytes that are in the buffer
        lseek(fd1, tanum, SEEK_SET);//starts from the start (SEEK_SET) t and goes to tantum
    }
    close(nfile);
    close(fd1);
    printf("\n");
}


void find(){
    if(! files_exist()) return;
    char buffercomp[BSIZE];
    printf("Enter lookup text: ");
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


void about(){
    printf("\n");
    printf("******************************************************\n");
    printf("*                                                    *\n");
    printf("*                    CATALOG                         *\n");
    printf("*                                                    *\n");
    printf("*        A program to manage binary files            *\n");
    printf("*               Using system calls                   *\n");
    printf("*                                                    *\n");
    printf("*                    CREATORS                        *\n");
    printf("*     Nefeli Stefanatou, Konstantinos Lazaros        *\n");
    printf("*                                                    *\n");
    printf("*             UNIVERSITY OF THESSALY                 *\n");
    printf("*                                                    *\n");
    printf("*          Department of computer science            *\n");
    printf("*            and biomedical informatics              *\n");
    printf("*                                                    *\n");
    printf("******************************************************\n");
}


int main(){
    about();
    int choice = 0;
    while(choice != 6){
        //system("@cls||clear");
        //printf("Selection menu: \n");
        choice = 0;
        printf("[1]Insert ");
        printf("[2]Edit ");
        printf("[3]Delete ");
        printf("[4]Find ");
        printf("[5]Display all ");
        printf("[6]Exit ");
        printf(" ~> ");
        if (1 != scanf(" %1d", &choice)){
            printf("Invalid choice. Exiting\n");
            break;
        }
        switch (choice){
            case 0:
                printf("Invalid choice");
                break;
            case 1:
                insert();
                break;
            case 2:
                edit();
                break;
            case 3:
                delete();
                break;
            case 4:
                find();
                break;
            case 5:
                display_all();
                break;
            default:
                choice = 6;
                printf("Invalid choice. Exiting\n");
                break;
        }
    }
    printf("\nThank you for using catalog ;-)\n");
    return 0;
}
