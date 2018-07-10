#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <string.h>

int main(){

    //Create a child process and pass a message to its parent using a shared memory location
    //the parent should then print the message.

    const int SIZE = 4096;
    void* ptr;

    ptr = mmap( NULL, SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0 );

    if( ptr == MAP_FAILED){

        printf( "Map Failed: %s", strerror(errno) );

    }

    pid_t pid_a;
    pid_a = fork();

    switch( pid_a ){

        case -1:

            // This is a fail state
            perror( "Error forking child a: ");
            return -1;

        case 0:

            //child process

            /*
             * This is the book method to create a general shared map
             * however, given that this is a child process, we can skip almost all of it
             * in favor of using an anonymous map that is not file-backed

            //Create the file descriptor
            sh_fd = shm_open( NAME, O_CREAT | O_RDWR, 0666 );

            //size it
            ftruncate( sh_fd, SIZE );

            //grab a pointer to it.
            ptr = mmap( NULL, SIZE, PROT_WRITE, MAP_SHARED, sh_fd, 0 );
            */


            //TODO: implement buffer pointer
            sprintf( (char*) ptr, "%s", "I am awesome" );
            break;

        default:

            wait( &pid_a );

            //TODO: Change to a file write, later and implement a buffer counter
            printf( "test: %s\n", (char*) ptr );

            int rtn = munmap( ptr, SIZE);

            if (0 != rtn){

                printf( "Error unmapping memory: %s", strerror(errno) );

            }


            break;
    }

    return 0;
}