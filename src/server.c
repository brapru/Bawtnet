#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void asciiArt(void){
#include "asciilogo.h"

        char *buf = malloc(1024*16);

        snprintf(buf, 1024*16, ascii_logo, 
                KRED, 
                KNRM, KRED, 
                KNRM, KRED, 1337,
                KNRM, KRED, 
                KNRM, KRED, (long) getpid()
        );
  
        printf("%s", buf);

        free(buf);
}

int main(void){
  
  asciiArt();

  return 0;
}
