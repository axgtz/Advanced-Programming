//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-22.
//

#include "Linked-Lists.h"

LinkedList(){ //    Constructor

}

int listLength(node * head){
    if(head == NULL) return 0;  // If list is empty
    int length;
    node * nd = head;
    printf("%d ",nd->data);

    while(nd->nextNode !=  NULL){
        nd = nd->nextNode;
        printf("%d ",nd->data);
    }
    printf("\n");
    return length;
}

int searchList(int numToSearch, node * Head) {   // looks for a certain number in the list returns position
    if(Head == NULL) return 0;

}

node * createNode(){
    node * n = NULL;
    n = malloc(sizeof(node));     // Allocate memory for the node
    n->nextNode = NULL;

    return n;
}

// Does not matter if list is empty as it just assigns head as the next node
void insertAtHead(int num,node * head){
    node * n = createNode();

    n->data = num;

    n->nextNode = head;

    head = n;
}
void insertAtTail(int num){

}

void insertAtPosition(int num, int pos){

}

bool deleteAtHead() {
    /*if (listLength() > 0) {

        //free(node->data);
        //free(node);
        return true;
    } else{
        return false;
    }*/
}

bool deleteAtTail(int num){

}

bool deleteAtPosition(int num, int pos);

void printList(){
    /*
    while(){

    }*/
}

