#ifndef ASL_H
#define ASL_H

#include "system.h"

struct semd_t semd_table[MAXPROC];
struct list_head * semdFree_h;
struct list_head semdFree;
struct list_head asl;
struct list_head * semd_h;

/* ASL handling functions */
semd_t* getSemd(int *key); /* Restituisce il puntatore al SEMD nella ASL la cui chiave è key (se non esiste NULL) */
void initASL(); /*Initialize Semaphore Descriptor (semd_t) */

int insertBlocked(int *key,pcb_t* p); /* Viene inserito il PCB p nella coda dei processi bloccati associata al SEMD con chiave key se il semaforo non è presente nella ASL alloca un nuovo SEMD alla lista di quelli liberi e lo inserisce nella ASL se non è possibile allocare un nuovo SEMD restituisce TRUE, in tutti gli altri casi FALSE */
pcb_t* removeBlocked(int *key); /* Ritorna il primo PCB dalla coda dei processi bloccati associati al SEMD con chiave key se non esiste tale descrittore ritorna NULL atrimenti ritorna l'elemento rimosso, se la coda dei processi diventa vuota rimuove il descrittore e lo inseirsce tra quelli liberi */
pcb_t* outBlocked(pcb_t *p); /* rimuove il PCB p dalla coda del semaforo su cui è bloccato. Se in tale semaforo no è presente p allora restituisce NULL altrimenti restituisce p. Se la coda dei processi del semaforo di appartenenza di p si svuota allora lo aggiunge alla coda dei semafori liberi */
pcb_t* headBlocked(int *key); /* restituisce (senza rimuovere) il puntantore al PCB che si trova in testa alla coda dei processi associata al SEMD di chiave key. Ritorna NULL se il SEMD non compare nella ASL o se compare ma la sua coda è vuota */
void outChildBlocked(pcb_t *p); /* rimuove il PCB puntato da p dalla coda del semaforo su cio è bloccato inoltre elimina tutti i processi dell'albero radicato in p dalle code dei semafori su cio sono bloccate */
void eraseTree(pcb_t* t);

#endif
