#include "termread.h"
#include "printput.h"
#include "terminal.h"
#include "system.h"

int main(void)
{
    char str[50];
    char *n = str;
    term_puts("Inserisci qua sotto la frase da stampare!\n");
    term_read(n,sizeof(str));
    if(Print_puts(n))
        term_puts("\nOperazione completata!");
    else
        term_puts("\nErrore di stampa");    

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

