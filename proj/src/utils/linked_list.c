#include "../mvc/controller/controller.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create a new doubly linked list
struct DoublyLinkedList* create_doubly_linked_list() {
    DoublyLinkedList* newList = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (newList == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}


Node* createNode(struct Board board) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->board = board;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}
void initializeList(DoublyLinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void insertAtBeginning(DoublyLinkedList* list, struct Board board) {
    Node* newNode = createNode(board);
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    list->size++;
}

void insertAtEnd(DoublyLinkedList* list, struct Board board) {
    Node* newNode = createNode(board);
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
    list->size++;
}

void insertAtPosition(DoublyLinkedList* list, struct Board board, int position) {
    if (position < 0 || position > list->size) {
        printf("Invalid position\n");
        return;
    }
    if (position == 0) {
        insertAtBeginning(list, board);
    } else if (position == list->size) {
        insertAtEnd(list, board);
    } else {
        Node* newNode = createNode(board);
        Node* current = list->head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;
        list->size++;
    }
}

void printList(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        
        printf("Board: \n");
        current = current->next;
    }
    printf("\n");
}