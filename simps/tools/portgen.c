#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//Port Generator by Forsaken.
int main(void) {
    const int port_max = 65535;
    srand((unsigned )time(NULL));
    printf("%d\n", rand() % port_max);
    return 0;
}