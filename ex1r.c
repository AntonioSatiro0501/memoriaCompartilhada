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

void identificaPalindromo(char *palavra) {
   int tamanho = strlen(palavra);
   for (int i = 0; i < tamanho / 2; i++) {
      if (palavra[i] != palavra[tamanho - i - 1]) {
         printf("Palavra '%s' não é palíndromo\n", palavra);
         return;
      }
   }
   printf("Palavra '%s' é palíndromo\n", palavra);
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
   
   /* Transfer blocks of data from shared memory to stdout*/
   while (shmp->complete != 1) {
      printf("segment contains : \n\"%s\"\n", shmp->buf);
      if (shmp->cnt == -1) {
         perror("read");
         return 1;
      }
      printf("Reading Process: Shared Memory: Read %d bytes\n", shmp->cnt);
      sleep(3);
   }

   identificaPalindromo(shmp->buf);

   printf("Reading Process: Reading Done, Detaching Shared Memory\n");
   if (shmdt(shmp) == -1) {
      perror("shmdt");
      return 1;
   }
   printf("Reading Process: Complete\n");
   return 0;
}