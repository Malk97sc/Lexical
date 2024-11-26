//send this file to lexical
#include <stdio.h>
#include <stdlib.h>

struct hi{
    int num;
};


int main(int argc, char *argv[]){

    int a = 2e-2;
    int b;

    a = b << 1;

    for(int i=0; i < a; i++){
        b += i;
        printf("%d\n", b);
    }

    while(b < a){
        b++;
    }
    return 0;
}