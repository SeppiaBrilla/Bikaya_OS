#ifndef PCB_H
#define PCB_H
#include "types_bikaya.h"
#include "listx.h"
#include "const.h"

/* PCB handling functions */

struct list_head * pcbfree_h;
struct list_head pcbfree;
struct pcb_t pcbFree_table[MAXPROC];

/* PCB free list handling functions */
void initPcbs(void); /* Inizializza la pcbFree in modo da contenere tutti gli elementi della Free_table */
void freePcb(pcb_t *p); /* Inserisce il PCB puntato da p nella lista dei PCB liberi */
pcb_t *allocPcb(void); /* Restituisce NULL se la pcbFree è vuota altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi a 0 e restituisce l'elemento rimosso */

/* PCB queue handling functions */
void mkEmptyProcQ(struct list_head *head); /* Inizializza la lista dei PCB inizializzando la sentinella */
int emptyProcQ(struct list_head *head); /* Restituisce true se la lista puntanta da head è vuota, False altrimenti */
void insertProcQ(struct list_head *head, pcb_t *p); /* Inserisce l'elemento puntato da p nella coda dei processi puntata da head (si deve tener conto della priorità decrescente) */
pcb_t *headProcQ(struct list_head *head); /* Restituisce l'elemento di testa della coda senza rimuoverlo, ritorna NULL se non ci sono elementi */

pcb_t *removeProcQ(struct list_head *head); /* Rimuove il primo elemento della coda dei processi puntata da head. Ritorna NULL se la coda è vuota altrimenti ritorna il puntatore all'elemento rimosso */
pcb_t *outProcQ(struct list_head *head, pcb_t *p); /* Rimuove il processo puntato da p, restituisce NULL se non presente (non importa in che posizione sia p) */


/* Tree view functions */
int emptyChild(pcb_t *this); /* Restituisce TRUE se il PCB this è una foglia */
void insertChild(pcb_t *prnt, pcb_t *p); /* Inserisce il PCB p come figlio di prnt (l'albero non è binario quindi c'è una lista di figli) */
pcb_t *removeChild(pcb_t *p); /* Rimuove il primo figlio di p,restituisce NULL se non ci sono figli */
pcb_t *outChild(pcb_t *p); /* Rimuove il PCB puntato da p dalla lista dei figli del padre, se P non ha un padre restituisce NULL altrimenti restituisce p */

#endif
