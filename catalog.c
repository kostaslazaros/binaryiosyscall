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
#define NAMES_FIRST 1  // Εάν στο αρχείο είναι πρώτα τα ονόματα και μετά τα επώνυμα


void printw(char *val){ //
   write(1, val, strlen(val));
}


int files_exist(){
    if((access(DATAFILE, F_OK) == -1) && (access(POSFILE, F_OK) == -1) ){
        printw("\nFiles do not exist. Insert a new record first ;-D\n");
        return 0;
    }
    return 1;
}


void flip(char *str){//flips name and surname for one entry only
    char sep = ' ';
    unsigned int stop;
    char fs[2][BSIZE];//makes an array to hold names and surnames
    memset(fs[0], '\0', BSIZE*sizeof(char));//clears the name buffer
    memset(fs[1], '\0', BSIZE*sizeof(char));//clears the surname buffer
    int selector = 0;
    int first = 0;
    for (stop=0; str[stop]; stop++){//till str has characters
        if (str[stop] == sep && selector == 0){//if we meet space and selector is 0- do exmample to undestnd better
            selector = 1;
            first = stop + 1;
            continue;
        }
        fs[selector][stop-first] = str[stop];//puts name and surname into fs[1] and fs[0] so seperates them
    }

    int pos = 0;
    for (int i=0; fs[1][i]; i++){
        str[i] = fs[1][i]; //puts surnames to str
        pos++;
    }
    if (pos > 0){
        str[pos] = ' ';//go to str and put space where you were
        pos++;
    }
    for (int j=0; fs[0][j]; j++){//from where we were after we put surname and space we put name
        str[pos+j] = fs[0][j];
    }
}

void flip_array(char arr[][128], int arrsize){//flips many times for all entries
    for (int i=0; i<arrsize;i++){//takes all entries and flips them
        flip(arr[i]);
    }
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

static int cmp_str(const void *lhs, const void *rhs){//used only by qshort-callback function
    return strcmp(lhs, rhs);
}


void sort_list(){
    if(! files_exist()) return;
    int counter = get_number_of_records();
    char bfr[counter][BSIZE];
    load_data(bfr);
    // put surnames first in order to sort list by surname
    if(NAMES_FIRST)
        flip_array(bfr, counter);
    qsort(bfr, counter, sizeof bfr[0], cmp_str);
    // revert list to normal
    if(NAMES_FIRST)
        flip_array(bfr, counter);
    save2file(bfr, counter);
}


int insert(){
    char entry[BSIZE];
    printw("Enter the new entry here: ");
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
    printf("\nRecord %s inserted successfully :-)\n", entry);
    return 0;
}


void delete(){
    if(! files_exist()) return;
    int counter = get_number_of_records();
    char bfr[counter][BSIZE];
    load_data(bfr);
    printw("\nList of Records\n");
    printw("---------------\n");
    for (int j=0; j<counter; j++){
        printf("%i.%s \n", j+1, bfr[j]);
    }
    int todelete;
    printw("\nSelect [record number] to delete or [0] to cancel: ");
    if (1 != scanf(" %d", &todelete)){
        printw("Invalid record number\n");
        return;
    };
    if(todelete > counter){
        printf("Number (%d) exceeds max record number (%d)\n", todelete, counter);
        return;
    }
    if(todelete == 0){
        printw("Delete cancelled\n");
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
    printf("\nRecord No: %d deleted :-(\n", todelete);
}


void edit(){
    if(! files_exist()) return;
    int counter = get_number_of_records();
    char bfr[counter][BSIZE];
    load_data(bfr);
    printw("\nList of Records\n");
    printw("---------------\n");
    for (int j=0; j<counter; j++){
        printf("%i.%s \n", j+1, bfr[j]);
    }
    int toedit;
    printw("\nSelect [record number] to edit or [0] to cancel: ");
    if (1 != scanf(" %d", &toedit)){
        printw("Invalid record number\n");
        return;
    };
    if(toedit > counter){
        printf("Number (%d) exceeds max record number (%d)\n", toedit, counter);
        return;
    }

    if(toedit == 0){
        printw("Edit cancelled\n");
        return;
    }
    printf("Record to Edit: %s\n", bfr[toedit-1]);
    printw("New Value     : ");
    memset(bfr[toedit-1], '\0', BSIZE*sizeof(char));
    scanf(" %[^\n]", bfr[toedit-1]);
    if(NAMES_FIRST)
        flip_array(bfr, counter);
    qsort(bfr, counter, sizeof bfr[0], cmp_str);
    if(NAMES_FIRST)
        flip_array(bfr, counter);
    save2file(bfr, counter);
    printf("\nRecord No: %d edited :-D\n", toedit);
}


void display_all(){
    if(! files_exist()) return;
    char buffer[BSIZE];
    int anum;
    int tanum = 0;
    int fd1 = open(DATAFILE, O_RDONLY); //file descriptor that shows that file opened
    int nfile = open(POSFILE, O_RDONLY);
    int record_no = 1;
    printw("\nList Of Records\n");
    printw("---------------\n");
    while(read(nfile, &anum, sizeof(int))){ //read the file that file decriptor shows,read sizeof(int) bytes a time and put them into the adress of anum//
        lseek(nfile, 0, SEEK_CUR); //leaves us at the position we were the last time we read
        printf("%i.", record_no++);
        memset(buffer, '\0', BSIZE*sizeof(char));
        tanum += anum;//total bytes read
        read(fd1, buffer, anum); //read the file that fd1 describes, read anum bytes a time and put them in buffer
        printf("%s\n", buffer); //print the bytes that are in the buffer
        lseek(fd1, tanum, SEEK_SET); //starts from the start (SEEK_SET) t and goes to tanum
    }
    close(nfile);
    close(fd1);
    printw("\n");
}


void search(){
    if(! files_exist()) return;
    char buffercomp[BSIZE];
    printw("Enter lookup text: ");
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
        if(NAMES_FIRST)
            flip(buffer); // Put Surname first
        if(strncmp(buffer, buffercomp, strlen(buffercomp)) == 0){
            if(NAMES_FIRST)
                flip(buffer); // Put name first again
            printf("\nFound entry: %s :-D\n", buffer);
            found = 1;
            break;
        }
        lseek(fd1, tanum, SEEK_SET);//starts from the start (SEEK_SET) t and goes to tantum
    }
    close(nfile);
    close(fd1);
    if(!found){
        printw("\nEntry not found :-(\n");
    }
}


void about(){
    printw("\n");
    printw("******************************************************\n");
    printw("*                                                    *\n");
    printw("*                    CATALOG                         *\n");
    printw("*                                                    *\n");
    printw("*        A program to manage binary files            *\n");
    printw("*               Using system calls                   *\n");
    printw("*                                                    *\n");
    printw("*                    CREATORS                        *\n");
    printw("*     Nefeli Stefanatou, Konstantinos Lazaros        *\n");
    printw("*                                                    *\n");
    printw("*             UNIVERSITY OF THESSALY                 *\n");
    printw("*                                                    *\n");
    printw("*          Department of computer science            *\n");
    printw("*            and biomedical informatics              *\n");
    printw("*                                                    *\n");
    printw("******************************************************\n");
}


int main(){
    about();
    // Αντί της printf μπορεί κανείς να χρησιμοποιήσει το παρακάτω:
    // char buff[] = "Hello People\n";
    // write(1, buff, strlen(buff));
    int choice = 0;
    while(choice != 6){
        //system("@cls||clear");
        //printf("Selection menu: \n");
        choice = 0;
        printw("[1]Insert ");
        printw("[2]Edit ");
        printw("[3]Delete ");
        printw("[4]Search ");
        printw("[5]Display all ");
        printw("[6]Exit ");
        printw(" ~> ");
        if (1 != scanf(" %1d", &choice)){
            printw("Invalid choice. Exiting\n");
            break;
        }
        switch (choice){
            case 0:
                printw("Invalid choice");
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
                search();
                break;
            case 5:
                display_all();
                break;
            case 6:
                break;
            default:
                choice = 6;
                printw("Invalid choice. Exiting\n");
                break;
        }
    }
    printw("\nThank you for using catalog ;-)\n");
    return 0;
}
