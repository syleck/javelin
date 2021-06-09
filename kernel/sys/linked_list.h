#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct ll_entry {
    struct ll_entry* next;
    struct ll_entry* previous;
    void* data;
} linkedlist;

// simply allocates a new ll entry and initializes variables
linkedlist* ll_create(void* first_entry);
// returns NULL if not found
linkedlist* ll_get(linkedlist* list, int id);
// returns first entry of the linked list from an entry
linkedlist* ll_getfirst(linkedlist* entry);
// destroys an entry of the list
void ll_destroy(linkedlist* entry);
// get the length of a list
int ll_len(linkedlist* list);
// inserts a new entry into the list
void ll_insert(linkedlist* list, int location, void* data); // if -1, insert to last position

#endif