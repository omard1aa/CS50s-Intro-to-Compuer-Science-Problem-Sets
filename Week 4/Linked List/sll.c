#include "stdio.h"
#include "stdio.h"
#include "stdlib.h"
#include "cs50.h"
#include "sll.h"

int main (int args, char* argv[]){
    printf("%d \n",'z');
    char c = 'a';
    int asci = (int) c;
    printf("%i\n", asci - 97);
    node* linkedlist = create(4);
    node *head = insert(linkedlist, 3);
    head = insert(head, 2);
    head = insert(head, 1);
    head = insert(head, 0);
    display(head);
    head = delete(head, 5);
    display(head);
}