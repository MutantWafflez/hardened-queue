#include "linkedlist.h"

LinkedList* CreateList(ListComparatorFunction compare, ListPrinterFunction print, ListDeleterFunction delete) {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->comparator = compare;
    list->printer = print;
    list->deleter = delete;
    list->length = 0;
    list->head = NULL;
    return list;
}

void InsertAtHead(LinkedList* list, void* valRef) {
    if(list == NULL || valRef == NULL)
        return;
    if (list->length == 0) list->head = NULL;

    ListNode** head = &(list->head);
    ListNode* newNode;
    newNode = malloc(sizeof(ListNode));

    newNode->data = valRef;
    newNode->next = *head;

    *head = newNode;
    list->length++;
}

void InsertAtTail(LinkedList* list, void* valRef) {
    if (list == NULL || valRef == NULL)
        return;
    if (list->length == 0) {
        InsertAtHead(list, valRef);
        return;
    }

    ListNode* head = list->head;
    ListNode* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(ListNode));
    current->next->data = valRef;
    current->next->next = NULL;
    list->length++;
}

void* RemoveAtHead(LinkedList* list) {
    if (list->length == 0) {
        return NULL;
    }

    ListNode* nextNode = list->head->next;
    void* headData = list->head->data;
    free(list->head);

    list->head = nextNode;
    list->length--;

    return headData;
}

bool InList(LinkedList* list, const void* data) {
    if (list->length == 0) {
        return false;
    }

    for (ListNode* curNode = list->head; curNode != NULL; curNode = curNode->next) {
        if (list->comparator(curNode->data, data) == 0) {
            return true;
        }
    }

    return false;
}

void PrintLinkedList(const LinkedList* list, FILE* fp) {
    if(list == NULL)
        return;

    ListNode* head = list->head;
    while (head != NULL) {
        list->printer(head->data, fp);
        fprintf(fp, "\n");
        head = head->next;
    }
}

void DestroyLinkedList(LinkedList** listRef) {
    ListNode* curNode = (*listRef)->head;
    while (curNode != NULL) {
        ListNode* nextNode = curNode->next;
        (*listRef)->deleter(curNode->data);
        free(curNode);
        curNode = nextNode;
    }

    free(*listRef);
    *listRef = NULL;
}