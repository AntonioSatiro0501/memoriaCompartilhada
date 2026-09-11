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
#define SHM_KEY 0x5555

//Escreva um programa que use memória compartilhada para inverter uma palavra fornecida pelo usuário.
//Antônio Costa Satiro de Souza 10723636

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

void invertePalavra(char *palavra){
    int tamanho = strlen(palavra);
    printf("Palavra '%s' invertida: ", palavra);
    for (int i = tamanho - 1; i > -1 ; i--) {
        printf("%c", palavra[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
   int shmid;
   struct shmseg *shmp;
   shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Shared memory");
      return 1;
   }
      shmp = shmat(shmid, NULL, 0);
   if (shmp == (void *) -1) {
      perror("Shared memory attach");
      return 1;
   }
   
   while (shmp->complete != 1) {
      printf("segment contains : \n\"%s\"\n", shmp->buf);
      if (shmp->cnt == -1) {
         perror("read");
         return 1;
      }
      printf("Reading Process: Shared Memory: Read %d bytes\n", shmp->cnt);
      sleep(3);
   }

   invertePalavra(shmp->buf);

   printf("Reading Process: Reading Done, Detaching Shared Memory\n");
   if (shmdt(shmp) == -1) {
      perror("shmdt");
      return 1;
   }
   printf("Reading Process: Complete\n");
   return 0;
}