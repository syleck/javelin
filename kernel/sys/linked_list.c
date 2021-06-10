#include "linked_list.h"
#include "../module.h"
#include "../mem.h"

MODULE("LINKEDLIST")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

linkedlist* ll_create(void* first_entry) {
    linkedlist* ll = (linkedlist*)malloc(sizeof(linkedlist));
    ll->data = first_entry;
    ll->next = 0;
    ll->previous = 0;
    return ll;
}

linkedlist* ll_get(linkedlist* list, int id) {
    linkedlist* truelist = ll_getfirst(list);
    for(int i = 0; truelist->next != 0; i++) {
        if(i == id) {
            return truelist;
        }
        truelist = truelist->next;
    }
    return NULL;
}

linkedlist* ll_getfirst(linkedlist* entry) {
    while(entry->previous != 0) {
        entry = entry->previous;
    }
    return entry;
}

void ll_destroy(linkedlist* entry) {
    entry->previous->next = entry->next;
    entry->next->previous = entry->previous;
}