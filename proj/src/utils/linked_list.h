#include <stdio.h>
#include <stdlib.h>

#pragma once

struct Board board;

typedef struct Node {
    struct Board board;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} DoublyLinkedList;

struct DoublyLinkedList* create_doubly_linked_list();
Node* createNode(struct Board board);

void initializeList(DoublyLinkedList* list);
void insertAtBeginning(DoublyLinkedList* list, struct Board board);
void insertAtEnd(DoublyLinkedList* list, struct Board board);
void insertAtPosition(DoublyLinkedList* list, struct Board board, int position);

void printList(DoublyLinkedList* list);