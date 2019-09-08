/*
    Simple linked list that stores integers
    
Roberto Alejandro Gutierrez Guillen A01019608
*/

#include <stdio.h>
#include "linked_list.h"

// Function declarations

// Insertion
node_t * insertHead (node_t * head, int data){
    node_t * n = createNode(data);

    // Now the new created node points to the previous first, could be NULL
    n->next = head;

    return n; // Return the new head
}

node_t * insertAtPosition (node_t * head, int data, int position){
    if(position > getLength(head)){
        printf("Position is bigger than the size of the list\n");
        return head;     // If the position is more than the length
    }
    if(position == 0)return insertHead(head,data);         // If the position is at the head, lets call insert head

    node_t * newNode = createNode(data);

    node_t * temp = head;

    for (int i = 1; i < position; ++i) { // Go to the desired position, starts in the head
        temp = temp->next;
    }

    newNode->next = temp->next; // Assign next node to the new one
    temp->next = newNode;       // Assign previous node to the new one

    return head;
}

// Deletion
node_t * deleteHead(node_t * head){
    if (head == NULL) return NULL;
    node_t * temp = head->next;
    free(head);

    return temp; // return the new head
}

node_t * deleteFromPosition(node_t * head, int position){ // TODO
    if(head == NULL) return head;   // List empty
    if(position > getLength(head)){
        printf("Position is bigger than the size of the list\n");
        return head;     // If the position is more than the length
    }
    if(position == 0)return deleteHead(head);         // If the position is at the head, lets call insert head

    node_t * temp = NULL;    // Saves the node to free
    node_t * prev = head->next;         // Saves the prev node

    for (int i = 1; i < position - 1; ++i) { // Go to one before desired position, starts in the head
        prev = prev->next;
    }

    temp = prev->next;
    prev->next = prev->next->next;   // Assign to the previous node the next node of the one being deleted

    free(temp); // Free desired node

    return head;
}

// Search
int searchIterative (node_t * head, int data){
    int cont = 0;
    node_t * temp = head;

    while (temp != NULL){ // Iterates till the end, could use find length
        if(temp->data == data){ // found it
            break;
        }
        temp = temp->next;
        cont++;
    }
    if(temp == NULL){
        return -1;
    }

    return cont;
}

// Helper methods
node_t * clear(node_t * head){ // TODO
    if(head == NULL) return head; // List is empty

    node_t * tempNode = head->next; // temp is always one ahead, so we can iterate while deleting the previous

    while (tempNode != NULL){
        free(head);
        head = tempNode;
        tempNode = tempNode->next;
    }
    free(head); // Free the last item

    return NULL;
}

void printList(node_t * head){
    printf("\tPrinting List with size: %i\n", getLength(head));
    if(head != NULL) {
        node_t *tempNode = head;

        while (tempNode != NULL) {
            printf("%d\n", tempNode->data);
            tempNode = tempNode->next;
        }
    }
    printf("\n");
}

void printRecursive(node_t * head){ // Recursive no tail
    if(head != NULL){
        printf(" %i \n", head->data);
        printRecursive(head->next);
    }
}

int getLength(node_t * head){
    if(head == NULL) return 0;  // If list is empty
    int len = 1;                // At least 1 node

    node_t * nd = head;

    while(nd->next !=  NULL){
        len++;
        nd = nd->next;
    }

    return len;
}

node_t * createNode(int data){
    node_t * n = NULL; // Init struct

    n = (node_t *) malloc(sizeof(node_t));  // Assign memory

    n->data = data; // Assign data

    return n;
}
