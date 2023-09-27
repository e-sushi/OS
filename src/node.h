/*

    Basic node implementations 

*/

#ifndef KERNEL_NODE_H
#define KERNEL_NODE_H

// doubly linked list node
struct LNode {
    LNode* prev;
    LNode* next;

    LNode() {}

    void
    insert_next(LNode* n) { 
        n->next = this->next;
        n->prev = n;
        n->next->prev = n;
        this->next = n; 
    }

    void
    insert_last(LNode* n) {
        n->prev = this->prev;
        n->next = this;
        n->prev->next = n;
        this->prev = n;
    }

    void
    remove() {
        this->next->prev = this->prev;
        this->prev->next = this->next;
    }
}; 

#endif // KERNEL_NODE_H