#include "disegno.h"


void init(){
    StartDrawing(20, 20);
    Point(5, 5);
    for(int i = 0; i < 1000; i++){
        ;
    }
    Point(3, 3);
    SYSCALL(3, 0, 0, 0);
}

