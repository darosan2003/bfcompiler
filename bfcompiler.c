#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 256
#define MAX 1024

void compiler(char *filename) {

  FILE *fp;

  unsigned int *memchunk, *beginning, *ending;
  char code[MAX];
  int i, braces; int err = 0;

  if((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "[-] Unable to open file %s\n", filename);
    fprintf(stderr, "[?] Perhaps you wrote it incorrectly\n");
    exit(3);
  }

  if((memchunk = calloc(sizeof(unsigned int), MEM_SIZE)) == NULL) {
    fprintf(stderr, "[-] Failed to allocate the memchunk on the heap\n");
    exit(4);
  }

  beginning = ending = memchunk;

  for(i=0; i<MEM_SIZE; i++)
    ending++;

  fscanf(fp, "%[^^]", code);

  for(i=0; i<strlen(code); i++) {
    if(code[i] == '[' || code[i] == ']')
      braces++;
  }

  if(braces % 2 != 0) {
    fprintf(stderr, "[-] Syntax Error. Missing '[' or ']'\n");
    fprintf(stderr, "[!] Aborting execution\n");
    err = 1;
  }

  for(i=0; i<strlen(code); i++) {
    if(err) break;

    switch(code[i]) {
      
      case '>':
        if(memchunk == ending) {
          fprintf(stderr, "[-] Pointer out of bounds. Position goes above 255\n");
          fprintf(stderr, "[!] Aborting execution\n");
          err = 1;
          break;
        }

        memchunk++;
        break;

      case '<':
        if(memchunk == beginning) {
          fprintf(stderr, "[-] Pointer out of bounds. Position goes below 0\n");
          fprintf(stderr, "[!] Aborting execution\n");
          err = 1;
          break;
        }

        memchunk--;
        break;

      case '+':
        if(*memchunk == 255) {
          fprintf(stderr, "[-] Incorrect Value. Value is above 255\n");
          fprintf(stderr, "[!] Aborting execution\n");
          err = 1;
          break;
        }

        (*memchunk)++;
        break;

      case '-':
        if(*memchunk == 0) {
          fprintf(stderr, "[-] Incorrect Value. Value is below 0\n");
          fprintf(stderr, "[!] Aborting execution\n");
          err = 1;
          break;
        }

        (*memchunk)--;
        break;

      case '.':
        printf("%c\n", *memchunk);
        break;

      case ',':
        scanf("%u", memchunk);

        if(*memchunk < 0 || *memchunk > 255) {
          fprintf(stderr, "[-] Incorrect Value. Value isn't between 0-255\n");
          fprintf(stderr, "[!] Aborting execution\n");
          err = 1;
          break;
        }

        break;

      case '[':
        if(*memchunk == 0) {
          while(code[i] != ']') {
            i++;
          } 
        }
        break;

      case ']':
        if(*memchunk != 0) {
          while(code[i] != '[') {
            i--;
          }
        }
        break;

    }
  }

  fclose(fp);
  free(beginning);

}

int main(int argc, char **argv) {

  if(argc != 2) {
    fprintf(stderr, "[-] Incorrect number of args supplied. Expected 2\n");
    fprintf(stderr, "[?] Usage: %s file.bf\n", argv[0]);
    return 1;
  }

  if(strncmp(argv[1] + strlen(argv[1]) - 3, ".bf", 3) != 0) {
    fprintf(stderr, "[-] Brainfuck file must have .bf extension\n");
    fprintf(stderr, "[?] Example: %s file.bf\n", argv[0]);
    return 2;
  }

  compiler(argv[1]);

  return EXIT_SUCCESS;

}
