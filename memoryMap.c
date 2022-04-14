#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /* wait */
#include <errno.h>	/* errno */
#include <time.h>       /* time */
#include <string.h>     /* string */

#define MSGSIZE 2

int main(int argc, char **argv) {
        int inbuf[MSGSIZE]; // buffer
        int pid;
        int total = 0; // hasil akhir
        int pidWait, status;
        
        // Use current time as seed for random generator
        srand(time(0));
        
        // printf("mulai \n");

        int size = MSGSIZE * sizeof(int);
        // buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        // int *shared = addr;
        int *randomNum = addr;

        int input = 0;
        printf("Berapa proses fork yang akan dijalankan?\n");
        scanf("%d", &input); // user input
        
        int i, j, k; // iterator
        
        for (i = 0; i < input; i++){
                switch (pid = fork()){
                case 0: /* proses anak */
                        printf("Proses konsumer %d \n", getpid());

                        randomNum[0] = rand()%50;
                        randomNum[1] = rand()%50; // storage random number
                        for (int k = 0; k<MSGSIZE; k++) {
                                printf("konsumer %d menghasilkan %d \n", getpid() , randomNum[k]);
                                total = total + randomNum[k];
                        }
                        break;
                default:        /* ortu */
                        /* wait sampai child selesai */
                        while (pidWait = wait(&status)){   
                                if (pidWait == pid){ /* child sukses selesai*/
                                        // cetak isi memory map
                                        for (int j = 0; j<MSGSIZE; j++) {
                                                printf("produser %d mengambil %d \n", getpid() , randomNum[j]);
                                                total = total + randomNum[j];
                                        }
                                        printf("total : %d \n", total);
                                        break; // keluar dari loop wait
                                }  

                                if ((pidWait == -1) && (errno != EINTR)) {
                                        /* ada error */
                                        perror("waitpid");
                                        exit(1);
                                }
                        }
                        break;
                case -1:        /* error */
                        perror("fork");
                        exit(1);
                }
        }
        exit(0);
}
