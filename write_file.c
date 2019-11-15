#include <unistd.h>
#include <stdlib.h>

int simply_write(){

    if((write(1, "Here is some data\n", 18)) != 18)
        write(2, "A writing error has occurred on file descriptor 1 \n", 46);

    exit(0);
}



