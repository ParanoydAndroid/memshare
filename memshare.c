#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(){

    //Create a child process and pass a message to its parent using a shared memory location
    //the parent should then print the message.

    const int SIZE = 4096;
    void* ptr = 0;

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

    ptr = mmap( NULL, SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0 );

    if( ptr == MAP_FAILED ){

        perror( "Map Failed: " );

    }

    pid_t pid_a;
    pid_a = fork();

    switch( pid_a ){

        case -1:

            perror( "Error forking child" );
            exit( EXIT_FAILURE );

        case 0:
            //child process

            ;
            char* msg = "I am awesome";
            int written = sprintf( (char*) ptr, "%s", msg );

            if ( written != strlen( msg ) ){

                printf( "Error writing msg to buffer\n" );

            }

            break;

        default:
            //parent

            ;
            FILE* out;
            out = fopen( "memshare.txt", "w");
            wait( &pid_a );

            fprintf( out, "My child has informed me: %s\n", (char*) ptr );

            int rtn = munmap( ptr, SIZE );

            if (0 != rtn){

                perror( "Error unmapping memory" );

            }

            break;

    }

    return 0;

}