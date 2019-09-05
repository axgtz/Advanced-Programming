//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-22.
//

#ifndef ADVANCED_PROGRAMMING_LINKED_LISTS_H
#define ADVANCED_PROGRAMMING_LINKED_LISTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct nodeLinkedList {
    int data;        // Can be upgraded to Template
    struct node * nextNode;
} node;

int listLength(node * head);

// if the number appears several times it will return the first one to find
int searchList(int numToSearch, node * head);   // looks for a certain number in the list returns position

node * createNode();  // For internal use, creates node returns pointer

void insertAtHead(int num, node * head);
void insertAtTail(int num);
void insertAtPosition(int num, int pos);

bool deleteAtHead();
bool deleteAtTail();
bool deleteAtPosition(int num, int pos);

void printList();

#endif //ADVANCED_PROGRAMMING_LINKED_LISTS_H
