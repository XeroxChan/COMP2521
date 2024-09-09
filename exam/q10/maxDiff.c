
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "BSTree.h"

int tree_traverse(BSTree node, int max_diff_value);
int count_node_left(BSTree node);
int count_node_right(BSTree node);

int maxDiff(BSTree t) {
    // TODO
    int max_diff_value = 0;
    //int num_nodes;
    if (t == NULL) {
        max_diff_value = 0;
    }
    else {
        max_diff_value = tree_traverse(t, max_diff_value);
        
    }
    return max_diff_value;
}


int tree_traverse(BSTree node, int max_diff_value) {
    if (node == NULL) {
        return 0;
    }
    else {

       // tree_traverse(node->left, max_diff_value);
       // tree_traverse(node->right, max_diff_value);
        
        int current_diff = abs(count_node_left(node->left) - count_node_right(node->right));
        //printf("key : %d\n", node->key);
        //printf("num_node_left : %d\n", count_node_left(node->left));
        //printf("num_node_right : %d\n", count_node_right(node->right));
        //printf("current_diff : %d\n", current_diff);
        
        
        if (max_diff_value - current_diff < 0) {
            max_diff_value = current_diff;
        }
        
        
        //printf("max_diff : %d\n", max_diff_value);
        tree_traverse(node->left, max_diff_value);
        tree_traverse(node->right, max_diff_value);
    }
    return max_diff_value;
}

int count_node_left(BSTree node) {
    if (node == NULL) {
        return 0;
    }
    
    return 1 + count_node_left(node->left) + count_node_right(node->right);
    
}

int count_node_right(BSTree node) {
    if (node == NULL) {
        return 0;
    }
    
    return 1 + count_node_left(node->left)+ count_node_right(node->right);
    
}
