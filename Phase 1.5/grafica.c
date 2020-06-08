#include "grafica.h"

#define TOD_LO     *((unsigned int *)BUS_REG_TOD_LO)
#define TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
#define RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define RAMTOP     (RAMBASE + RAMSIZE)

#define ST_READY         1
#define ST_BUSY          3
#define ST_TRANSMITTED   5
#define CMD_ACK          1
#define CMD_TRANSMIT     2
#define CHAR_OFFSET      8
#define TERM_STATUS_MASK 0xFF

#define SYS3       3
#define STEPS      6

volatile int test1_baton[STEPS + 1] = {0};
volatile int test2_baton[STEPS + 1] = {0};
volatile int test3_baton[STEPS + 1] = {0};

typedef unsigned int devreg;

static unsigned int get_microseconds() {
    return TOD_LO / TIME_SCALE;
}

static void delay_ms(unsigned int ms) {
    unsigned int start = get_microseconds();

    while (get_microseconds() - start <= ms * 1000)
        ;
}

/******************************************************************************
 * I/O Routines to write on a terminal
 ******************************************************************************/

#ifdef TARGET_UMPS
static termreg_t *term0_reg = (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, 0);

static unsigned int tx_status(termreg_t *tp) {
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

void termprint(char *str) {
    while (*str) {
        unsigned int stat = tx_status(term0_reg);
        if (stat != ST_READY && stat != ST_TRANSMITTED)
            return;

        term0_reg->transm_command = (((*str) << CHAR_OFFSET) | CMD_TRANSMIT);

        while ((stat = tx_status(term0_reg)) == ST_BUSY)
            ;

        term0_reg->transm_command = CMD_ACK;

        if (stat != ST_TRANSMITTED)
            return;
        else
            str++;
    }
}
#endif
#ifdef TARGET_UARM
#define termprint(str) tprint(str);
#endif


void StartDrawing(int x, int y){
    termprint("init start\n");
    ready = 0;
    change = 1;
    width = x;
    height = y;
    char *tmp[height + 2];
    char line[width + 3];
    for(int i = 0; i< height; i ++){
        for(int j = 0; j < width; j ++){
            line[j] = '.';
        }
        line[width] = '\n';
        line[width + 1] = '\0';
        tmp[i] = line;
    }
    tmp[height] = "\n";
    tmp[height + 1] = "\n";
    canvas = tmp;
    draw = 1;
    termprint("init done\n");
}

void Point(int x, int y){
    while(!draw){
        ;
    }
    while(!change){
        ;
    }
    ready = 0;
    if( width >= x &&  height >= y){
        canvas[y - 1][x - 1] = '&';
    }
    ready = 1;
}

void Draw(){
    while(!draw){
        ;
    }
    termprint("Draw\n");
    while(draw){
        while(!ready){
            //termprint("not ready\n");
        }
        change = 0;
        for (int i = 0; i <height + 2; i++){
            termprint(canvas[i]);
            //termprint("i");
        }
        change = 1;
    }
    SYSCALL(3, 0, 0, 0);

}