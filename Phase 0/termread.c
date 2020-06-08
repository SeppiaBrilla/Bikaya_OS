#include "system.h"
#include "terminal.h"

void term_read(char *n, const int length){
    int i = 0;
    do
    {
        n[i] = term_getchar();
        if(n[i] != -1){
            i++;
		}else{
			term_puts("errore!");		
		}
		if(i >= length-1){
			term_puts("errore, sequenza troppo lunga!");	
		}
    } while (i < length-1 && n[i-1] != '\n');
    n[i]=0;
} 
