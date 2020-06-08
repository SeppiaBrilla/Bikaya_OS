#include "const.h"
#include "listx.h"
#ifdef TARGET_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#endif

#ifdef TARGET_UARM
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#endif

#include "pcb.h"


#define black &
char **canvas;

int width;
int height;
int ready;
int change;
int draw;
int need;

void StartDrawing(int x, int y);
void Point(int x, int y);
extern void Draw();