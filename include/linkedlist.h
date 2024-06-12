#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct listnode_t {
    void* data;
    struct listnode_t* next;
} ListNode;

typedef int (*ListComparatorFunction)(const void*, const void*);
typedef void (*ListPrinterFunction)(const void*, FILE*);
typedef void (*ListDeleterFunction)(void*);

typedef struct {
    ListNode* head;
    int length;
    ListComparatorFunction comparator;
    ListPrinterFunction printer;
    ListDeleterFunction deleter;
} LinkedList;

LinkedList* CreateList(ListComparatorFunction, ListPrinterFunction, ListDeleterFunction);

void InsertAtHead(LinkedList* list, void* valRef);

void InsertAtTail(LinkedList* list, void* valRef);

void* RemoveAtHead(LinkedList* list);

bool InList(LinkedList* list, const void* data);

void PrintLinkedList(const LinkedList* list, FILE* fp);

void DestroyLinkedList(LinkedList** listRef);

#endif
