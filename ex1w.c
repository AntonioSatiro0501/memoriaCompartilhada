#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234

//Escreva um programa que use memória compartilhada para verificar se uma palavra fornecida pelo usuário é palíndrome ou não.

struct shmseg {
   int cnt;
   int complete;
   char buf[BUF_SIZE];
};

int fill_buffer(char * bufptr, int size, char palavra[40]) {
   int filled_count;
   memset(bufptr, 0, size);
   strncpy(bufptr, palavra, size - 1);
   bufptr[size-1] = '\0';
   filled_count = strlen(bufptr);
   return filled_count;
}


int main(int argc, char *argv[]) {

   struct shmseg *shmp;
   char *bufptr;
   int shmid, spaceavailable;
   char entrada[40];
   scanf("%39s", entrada);
   shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);

      if (shmid == -1) {
      perror("Shared memory");
      return 1;
   }
      // Attach to the segment to get a pointer to it.
   shmp = shmat(shmid, NULL, 0);
   if (shmp == (void *) -1) {
      perror("Shared memory attach");
      return 1;
   }
   /* Transfer blocks of data from buffer to shared memory */
   bufptr = shmp->buf;
   spaceavailable = BUF_SIZE;
   shmp->cnt = fill_buffer(bufptr, spaceavailable, entrada);
   shmp->complete = 0;
   printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", shmp->cnt);
   shmp->complete = 1;
   printf("Writing Process: Complete\n");
   return 0;
}