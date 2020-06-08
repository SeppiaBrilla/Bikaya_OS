#include "pcb.h"

void initPcbs(void){
    INIT_LIST_HEAD(&pcbfree); //inizializza ia lista
    pcbfree_h = &pcbfree;
    for(int i=0; i<MAXPROC; i++){ //vengono aggiunti i processi nella lista
        struct pcb_t * pcb = &(pcbFree_table[i]);
        list_add(&(pcb->p_next), pcbfree_h);
    }
}

void freePcb(pcb_t * p){
    list_add_tail(&(p->p_next), pcbfree_h);
}

pcb_t *allocPcb(void){
    if(list_empty(pcbfree_h)){
        return NULL;
    } else {
        struct list_head *p = pcbfree_h-> next;
        list_del(pcbfree_h->next); //rimuove un elemento
        struct pcb_t * pcb = container_of(p, pcb_t, p_next);
        pcb->priority=0; //inizializza i campi
        pcb->p_semkey=NULL;
        INIT_LIST_HEAD(&(pcb->p_child));
        INIT_LIST_HEAD(&(pcb->p_next));
        INIT_LIST_HEAD(&(pcb->p_sib));
        return pcb;
    }
}

void mkEmptyProcQ(struct list_head* head){
    INIT_LIST_HEAD(head);

}

int emptyProcQ(struct list_head* head){
    return list_empty(head);
}

void insertProcQ(struct list_head *head, pcb_t *p){
    if(list_empty(head))
        list_add(&(p->p_next),head);
    else
    {
        struct list_head *pcb;
        list_for_each(pcb, head) //controlliamo tutte le priorità per controllare dove inserire il pcb
        {
            pcb_t *q = container_of(pcb, pcb_t, p_next);
            if(p->priority > q->priority)
            {
                list_add(&(p->p_next),pcb->prev);
                pcb = head->prev;
            }
            else if(list_is_last(pcb, head)) //se è arrivato alla fine inserisce
            {
                list_add(&(p->p_next),pcb);
                pcb = head->prev;
            }
        }
    }
}

pcb_t *headProcQ(struct list_head *head){
    if(!list_empty(head))
        return container_of(head->next, pcb_t, p_next);
    else
        return NULL;
}

pcb_t *removeProcQ(struct list_head *head){
    if(list_empty(head))
        return NULL;
    else{
        struct list_head * p = head->next;
        list_del(head->next);	//rimuove e restituisce il primo pcb della lista
        return container_of(p, pcb_t, p_next);
    }
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p){
    struct list_head *x;
    list_for_each(x, head){
        if (container_of(x, pcb_t, p_next) == p) { //se troba p lo elimina
            list_del(x);
            return container_of(x, pcb_t, p_next);
        }
    }
    return NULL;
}

int emptyChild(pcb_t *this){
    return list_empty(&(this->p_child));
}

void insertChild(pcb_t *prnt, pcb_t *p){
    p->p_parent = prnt;
    list_add_tail(&(p->p_sib), &(prnt->p_child));
}

pcb_t *removeChild(pcb_t *p){
    if (!emptyChild(p)) //se la lista dei figli non è vuota rimuove il primo figlio
    {
        struct pcb_t *f = container_of((p->p_child).next,pcb_t,p_sib);
        list_del(p->p_child.next);
        return f;
    }
    else
        return NULL;

}

pcb_t *outChild(pcb_t *p){
    if(p->p_parent==NULL)
        return NULL;

    p->p_parent = NULL;
    list_del(&(p->p_sib));//rimuove p dalla lista dei fratelli
    return p;
}
