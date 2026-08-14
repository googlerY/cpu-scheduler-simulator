#include <stdlib.h>
#include "queue.h"

void initQueue(queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

P* dequeue(queue* q) {
    if (q->head == NULL) return NULL;
    
    node* temp = q->head;
    P* p = temp->process;
    
    q->head = temp->next;
    if (q->head != NULL) {
        q->head->prev = NULL;
    } else {
        q->tail = NULL;
    }
    
    free(temp);
    return p;
}

void enqueue(queue* q, P* p) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->process = p;
    newNode->next = NULL;
    newNode->prev = q->tail;
    
    if (q->tail != NULL) {
        q->tail->next = newNode;
    } else {
        q->head = newNode;
    }
    q->tail = newNode;
}

void enqueue_by_priority(queue* q, P* p) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->process = p;
    newNode->prev = NULL;
    newNode->next = NULL;
    
    node* curr = q->head;
    
    // 우선순위가 낮을수록(숫자가 클수록) 뒤로 보냄
    while (curr != NULL && curr->process->priority <= p->priority) {
        curr = curr->next;
    }
    
    if (curr == NULL) { 
        enqueue(q, p);
    } else if (curr == q->head) {
        newNode->next = q->head;
        q->head->prev = newNode;
        q->head = newNode;
    } else {
        newNode->next = curr;
        newNode->prev = curr->prev;
        curr->prev->next = newNode;
        curr->prev = newNode;
    }
}