#include "system.h"

#define PR_NOTINSTALLED    0
#define PR_READY           1
#define PR_ILLEGAL         2
#define PR_BUSY            3
#define PR_ERROR           4

#define CMD_ACK            1
#define CMD_PRINT          2
#define CMD_RESET          0

#define CHAR_OFFSET        8
#define TERM_STATUS_MASK   0xFF

static dtpreg_t *print0_reg = (dtpreg_t *) DEV_REG_ADDR(IL_PRINTER, 0);

static unsigned int tx_statusp(dtpreg_t *tps)
{
    return ((tps->status) & TERM_STATUS_MASK);
}

static int Print_putchar(char c)
{
    unsigned int stat;
    stat = tx_statusp(print0_reg);
    if(stat == PR_READY){
    	print0_reg->data0= (int) c;
	print0_reg->command = CMD_PRINT;
	while ((stat = tx_statusp(print0_reg)) == PR_BUSY)
	    ;

	stat = tx_statusp(print0_reg);
	if(stat == PR_ILLEGAL || stat == PR_ERROR){
		return -1;
	}
	print0_reg->command = CMD_ACK;
	stat = tx_statusp(print0_reg);
	if(stat != PR_READY){
		return -1;
	}
	return 0;
    }
    return -1;
}

int Print_puts(const char *str)
{
    while (*str){
        if (Print_putchar(*str++))
			return 0;
	}
	return 1;
}


