#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv) {
    for (int i = 0; i < 3; i++) {
        sleep(1);
        printf("hello!!!\n");
    }
    
    return 0;
}
