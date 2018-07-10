#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(){

    //Create a child process and pass a message to its parent using a shared memory location
    //the parent should then print the message.

    const int SIZE = 4096;
    const char* NAME = "sh_buffer";

    int sh_fd;
    void* ptr;
    pid_t pid_a;

    pid_a = fork();

    switch( pid_a ){

        case -1:

            // This is a fail state
            perror( "Error forking child a: ");
            return -1;

        case 0:

            //child process
            sh_fd = shm_open( NAME, O_CREAT | O_RDWR, 0666 );
            ftruncate( sh_fd, SIZE );
            ptr = mmap( NULL, SIZE, PROT_READ, MAP_SHARED, sh_fd, 0 );

            //TODO: implement buffer pointer
            sprintf( (char*) ptr, "%s", "test" );
            break;

        default:

            //parent process
            sh_fd = shm_open( NAME, O_CREAT | O_RDWR, 0666 );
            ftruncate( sh_fd, SIZE );
            ptr = mmap( NULL, SIZE, PROT_READ, MAP_SHARED, sh_fd, 0 );

            wait( &pid_a );

            //TODO: Change to a file write, later and implement a buffer counter
            printf( "%s", (char*) ptr );

            break;
    }

    return 0;
}