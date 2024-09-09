
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void traverse_l1(Node result, Node l1, int x);
void traverse_l2(Node result, Node l2, int y);

// Worst case time complexity of this solution: O(m + n)

List zipList(List l1, int x, List l2, int y) {
    // TODO
    
    List result = ListNew();
    
    
    Node current_1 = l1->first;
    Node current_2 = l2->first;
    
    
    if (l1 != NULL) {
        result->first = current_1;
        result->last = current_1;
    }
    else {
        result->first = current_2;
        result->last = current_2;
    }
    
    while (current_1->next != NULL && current_2->next != NULL) {
        traverse_l1(result->first, current_1, x);
        traverse_l2(result->first, current_2, y);
        
    }

    
    return result;
}

void traverse_l1(Node result, Node l1, int x) {
    int i = 0;
    while (i < x) {
        
        Node temp = newNode(l1->value);
        result->next = temp;

        l1 =  l1->next;
        
        i++;
    }
}


void traverse_l2(Node result, Node l2, int y) {
    int i = 0;
    while (i < y) {
         Node temp = newNode(l2->value);
        result->next = temp;

        l2 = l2->next;


        i++;
    }
}
