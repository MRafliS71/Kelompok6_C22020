#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>

/*
        Cahaya Aulia Firdausyah
        Muhammad Rafli Syaputra
        Muhammad Naufal Daffa
        Sri Nanda Ameliani
        Sukma Julianti
*/

#define MSGSIZE 10

int main(int argc, char **argv) {
        int inbuf[MSGSIZE]; //buffer
        int pid;
        int fd[2]; //pipe
        int temp = 0;
        int x = 0,y = 0;
       
        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }
        printf("mulai \n");
        switch (pid = fork()) {
        case 0:         /* fork returns 0 ke proses 1*/
                printf("Proses Input random number \n");
                // tulis data ke pipe
                int msg1[2]  = {rand()%20, rand()%10};
                int msg2[2]  = {rand()%20, rand()%10}; 

                /* tutup bagian input dari pipe */
                close(fd[0]);
                // tulis ke pipe
                write(fd[1], msg1, MSGSIZE);
                write(fd[1], msg2, MSGSIZE);
                break;
        default:        /* fork returns pid ke proses 2 */
                printf("Proses penjumlahan\n");
                /* tutup bagian output dari pipe */
                close(fd[1]);
                // baca yang ditulis child dari pipe
                read(fd[0], inbuf, MSGSIZE); //buffer terisi
                temp = inbuf[0] + inbuf[1];
                printf("proses menulis pesan ke-1: %d %d\n", inbuf[0], inbuf[1]);
                
                read(fd[0], inbuf, MSGSIZE); //buffer terisi
                temp = temp + inbuf[0] + inbuf[1];
                printf("proses menulis pesan ke-2: %d %d\n", inbuf[0], inbuf[1]);
                
                printf("hasil penjumlahan: %d \n", temp);
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
        return 0;
}
