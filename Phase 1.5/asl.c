#include "asl.h"

semd_t* getSemd(int *key){

    struct list_head *x;
    list_for_each(x, semd_h){		// cicla su tutti i semafori della ASL
        struct semd_t *sem = container_of(x,semd_t,s_next);
        if (sem->s_key == key) {	// quando trova il semaforo identificato dalla chiave passata
            return sem;			// ritorna il puntatore al semaforo
        }
    }
    return NULL;			// altrimenti NULL
}

void initASL(void){
    INIT_LIST_HEAD(&semdFree); //inizializza semdFree
    semdFree_h = &semdFree; //e la sua testa 
    INIT_LIST_HEAD(&asl);  // inizializza la ASL
    semd_h = &asl; //e la sua testa 
    for(int i=0; i<MAXPROC; i++){ //inizializza semdFree e va ad aggiungerci MAXPROC semafori liberi
        struct semd_t * sem = &(semd_table[i]);
        INIT_LIST_HEAD(&(sem->s_next));
        INIT_LIST_HEAD(&(sem->s_procQ));
        list_add(&(sem->s_next), semdFree_h);
    }
}

int insertBlocked(int *key, pcb_t* p){
    struct semd_t *semaphore = getSemd(key); //ottiene il semaforo a cui key è associata
    if(semaphore != NULL){
        insertProcQ(&(semaphore->s_procQ),p); //inserisce il processo nella procQ
    }
    else
    {
        if(list_empty(semdFree_h)) { 
            return TRUE;
        } else {
            struct semd_t *sem = container_of(semdFree_h->next,semd_t,s_next); //crea un nuovo semaforo se non ne esiste uno con la key associata 
            list_del(semdFree_h->next);
            list_add(&(sem->s_next),semd_h);

            insertProcQ(&(sem->s_procQ), p); //viene creato un semaforo con il processo nella queue
            sem->s_key = key; //e con la chiave ricercata

        }
    }
    p->p_semkey = key;
    return FALSE;
}

pcb_t* removeBlocked(int *key){
    struct semd_t* semd = getSemd(key);	// ottiene il semaforo identificato dalla key passata
    if (semd == NULL)
        return NULL;

    struct pcb_t* pcb = removeProcQ(&(semd->s_procQ));	// se esiste allora rimuove il primo processo della procQ
    
    if(list_empty(&(semd->s_procQ))){	// se dopo aver rimosso il processo non ve ne sono altri in coda
        list_del(&(semd->s_next));	// elimina il semaforo dalla ASL 
        list_add(&(semd->s_next),semdFree_h);	// rendendolo di nuovo libero
    }
    return pcb;
}

pcb_t* outBlocked(pcb_t *p){
    struct semd_t* semd=getSemd(p->p_semkey);	// ottiene il semaforo identificato da key

    if(semd==NULL)
        return NULL;

    struct pcb_t* cond=outProcQ(&(semd->s_procQ),p);	// se esiste, rimuove il processo puntato da p dalla procQ senza rimuoverlo
    if(list_empty(&(semd->s_procQ))){	// se dopo aver rimosso il processo non ve ne sono altri in coda
  	     list_del(&(semd->s_next));	// elimina il semaforo dalla ASL
  	     list_add(&(semd->s_next),semdFree_h);	// rendendolo di nuovo libero
    }
    return cond;
}

pcb_t* headBlocked(int *key){
    semd_t *semaphore = getSemd(key); //cerca il semd di chiave key
    if (semaphore == NULL || list_empty(&(semaphore->s_procQ))) //controlla non sia null e la procQ non sia vuota 
        return NULL; 

    return container_of((semaphore->s_procQ).next,pcb_t,p_next); //altrimenti ritorna il primo pcb
}

void outChildBlocked(pcb_t *p){
    struct pcb_t *child = container_of((p->p_child).next,pcb_t,p_sib); //ottiene il figlio 
    if(child != NULL){
        eraseTree(child); //richiama una funzione che ricorsivamente fa outblocked sull'albero radicato in p
    }
    outBlocked(p);
}

void eraseTree(pcb_t* t){
    if(outBlocked(t) != NULL){
        if((t->p_sib).next != &(t->p_sib)){ //ottiene il primo fratello se esiste 
            struct pcb_t *b =container_of((t->p_sib).next,pcb_t,p_sib);
            eraseTree(b); //chiamata ricorsiva 
        }
        struct pcb_t *child = container_of((t->p_child).next,pcb_t,p_sib); // chiamata ricorsiva sul figlio 
        eraseTree(child);
	outBlocked(t);
    }
}
