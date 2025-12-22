// commande a tapé : gcc main.c -o main; .\main.exe
//                   gcc -o main main.c; .\main.exe 
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world!\n");
    printf("coucou222");
    static int i = 0;
    while (1)
    {
        
        i++;
        printf("i = %f\n", (double)i);
    }
    return 0;
}