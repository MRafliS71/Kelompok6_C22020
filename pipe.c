#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>     /* string */
#include <time.h>       /* time */
#include <sys/wait.h>   /* wait */
#include <errno.h>	/* errno */

#define MSGSIZE 2

int main(int argc, char **argv) {
        int inbuf[MSGSIZE]; // buffer
        int pid;
        int pidWait, status; 
        int fd[2]; // pipe
        int total = 0; // hasil akhir

        // Use current time as seed for random generator
        srand(time(0));

        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }

        int input = 0;
        printf("Berapa proses fork yang akan dijalankan?\n");
        scanf("%d", &input); // user input
        
        int i, j, k; // iterator
        
        for (i = 0; i < input; i++){
                switch (pid = fork()){
                case 0:
                        printf("proses produser %d\n", getpid());
                        int randomNum[MSGSIZE] = {rand()%50, rand()%50}; // storage random number

                        /* tutup bagian input dari pipe */
                        // close(fd[0]);

                        // menampilkan apa yang telah ditulis oleh child
                        for (int k = 0; k < MSGSIZE; k++){
                                printf("proses produser %d menghasilkan: %d\n", getpid(), randomNum[k]);
                        }

                        // tulis ke pipe
                        // child menulis isi array dari random number
                        write(fd[1], randomNum, sizeof(randomNum));
                
                        break;
                default:        /* fork returns pid ke proses ortu */
                        while(pidWait = wait(&status)){
                                if (pidWait == pid){
                                        /* tutup bagian output dari pipe */
                                        close(fd[1]);
                
                                        // baca yang ditulis child dari pipe
                                        read(fd[0], inbuf, sizeof(inbuf));

                                        // perulangan untuk print yang telah dibaca oleh ortu
                                        for (j = 0; j < MSGSIZE; j++){
                                                printf("proses konsumer %d mengambil: %d \n", getpid(), inbuf[j]);
                                                // ortu membaca nilai dari anak
                                                // jika ortu membaca angka maka akan menjumlahkan dengan angka sebelumnya
                                                total = total + inbuf[j];

                                                // print nilai total yang telah ditambahkan
                                                printf("total jumlah : %d\n", total);
                                        }
                                        break;
                                }

                                if ((pidWait == -1) && (errno != EINTR)) {
                                        /* jika error */
                                        perror("waitpid");
                                        exit(1);
                                }                                
                        }                  
                case -1: /* jika case error */
                        perror("fork");
                        exit(1);
                }
        }
         
        // keluar
        exit(0);
}
