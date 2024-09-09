// Centrality Measures API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"



typedef struct node *Node;
struct node {
    int v;
    Node next;
};

struct queue {
    Node head;
    Node tail;
    int  size;
};

typedef struct queue *Queue;

//helper function
double find_reachable(Graph g, int index, int num_nodes);
static int find_num_paths(NodeData *pred, int j, PredNode *next);
static int find_num_pass_through(NodeData *pred, int j, PredNode *next, int i);

//queue functions for find_reachable
bool QueueIsEmpty(Queue q);
Queue QueueNew(void);
void QueueFree(Queue q);
void QueueEnqueue(Queue q, int v);
int QueueDequeue(Queue q);
static Node newNode(int v);
int QueueSize(Queue q);


double *closenessCentrality(Graph g) {
    //number of nodes in the graph
    double num_nodes = GraphNumVertices(g);
    
    //allocate memory for closeness array
    double *closeness = malloc(sizeof(double) * num_nodes);
    
    //initialize closeness array
    for (int i = 0; i < num_nodes; i++) {
       closeness[i] = 0.0;
    }
    
    //find number of nodes reachable from index
    for (int index = 0; index < num_nodes; index++) {
        
        //find number of nodes reachable from index
        double num_reachable = 0;
        //find reachable nodes from index
        num_reachable = find_reachable(g, index, num_nodes);
        
        double first_part = (num_reachable - 1) / (num_nodes - 1);
        
        double second_part = num_reachable - 1; 
        
        //summation of all shortest distance from index
        double sum = 0.0;
        NodeData *all_distance = dijkstra(g, index);
        for (int j = 0; j < num_nodes; j++) {
            double shortest_distance;
           //accumulate the distances
           //change all INFINITY distance to 0
            if (all_distance[j].dist == INFINITY) {
                shortest_distance = 0;
            }
            else {
                shortest_distance = all_distance[j].dist;
            }
            //all to sum
            sum = sum + shortest_distance;
        }
        //avoid division by 0 which is undefined
        if (sum == 0) {
            second_part = 0;
        }
        else {
            second_part = second_part / sum;
        }
        closeness[index] = first_part * second_part; 
    }
	return closeness;
}

double *betweennessCentrality(Graph g) {
    //number of nodes in the graph
    double num_nodes = GraphNumVertices(g);
    
    //allocate memory for closeness array
    double *betweenness= malloc(sizeof(double) * num_nodes);
    
    //initialize closeness array
    for (int i = 0; i < num_nodes; i++) {
       betweenness[i] = 0.0;
    }
    
    
    for (int index = 0; index < num_nodes; index++) {

        double sum = 0.0;
        for (int j = 0; j < num_nodes; j++) {
            //skip index's self NodeData array
             if (index == j) {
                continue;
            }
            //get the NodeData array for the rest of the graph
            NodeData *all_pred = dijkstra(g, j);
            
            //iterate through the pred arrays for the 
            for (int k = 0; k < num_nodes; k++) {
                //skip index as destination
                if (index == k) {
                    continue;
                }
                //find number of paths
                double num_paths = find_num_paths(all_pred, j, all_pred[k].pred);
                //find number of paths that pass through index
                double num_pass_through = find_num_pass_through(all_pred, j, all_pred[k].pred, index);
                //avoid division of 0
                if (num_paths != 0) {
                    double betw = num_pass_through / num_paths;
                    sum = sum + betw;
                }
                
            }       
            
        }
        betweenness[index] = sum;
    }
    
    
	return betweenness;
}








double find_reachable(Graph g, int index, int num_nodes) {
    double reachable = 0;
    int visited[num_nodes];
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = -1;
    }
    //create queue
    Queue q = QueueNew();
    //starting vertex
    QueueEnqueue(q, index);
                       
    while (!QueueIsEmpty(q)) {
        int curr = QueueDequeue(q);
        //if visited, go to next loop
        if (visited[curr] == 1) {
            continue;
        }
        //mark as visited
        visited[curr] = 1;
        
        //get the out_edges of curr
        AdjList out_edges = GraphOutIncident(g, curr);
        while (out_edges != NULL) {
            //if not yet visit
            if (visited[out_edges->v] == -1) {
                //enqueue
                QueueEnqueue(q, out_edges->v);
            }
            out_edges = out_edges->next;
        }
        
    }
    //count the number of ones in visited array
    for (int j = 0; j < num_nodes; j++) {
        if (visited[j] == 1) {
            reachable += 1;
        }
    }
    return reachable;
}


static int find_num_paths(NodeData *pred, int j, PredNode *next) {
    PredNode *curr = next;
    //base case
    if (curr == NULL) {
        return 0;
    }
    if (curr->v == j) {
        return 1 + find_num_paths(pred, j, curr->next);
    }
    return find_num_paths(pred, j, pred[curr->v].pred) + find_num_paths(pred, j, curr->next);
}


static int find_num_pass_through(NodeData *pred, int j, PredNode *next, int i) {
    PredNode *curr = next;
    //base case
    if (next == NULL) {
        return 0;
    }
    else if (curr->v == j) {
        return find_num_pass_through(pred, j, curr->next, i);
    }
    
    else if (curr->v == i) {
        return 1 + find_num_pass_through(pred, j, curr->next, i);
    } 
    
    else {
        return find_num_pass_through(pred, j, pred[curr->v].pred, i) + find_num_pass_through(pred, j, curr->next, i);
    }
}





/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
    Queue q = malloc(sizeof(*q));
    if (q == NULL) {
        fprintf(stderr, "couldn't allocate Queue\n");
        exit(EXIT_FAILURE);
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}


static Node newNode(int v) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "couldn't allocate Node\n");
        exit(EXIT_FAILURE);
    }

    n->v = v;
    n->next = NULL;
    return n;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
    Node curr = q->head;
    while (curr != NULL) {
        Node temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, int v) {
    Node n = newNode(v);
    if (q->size == 0) {
        q->head = n;
    } else {
        q->tail->next = n;
    }
    q->tail = n;
    q->size++;
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
int QueueDequeue(Queue q) {
    assert(q->size > 0);

    Node newHead = q->head->next;
    int v = q->head->v;
    free(q->head);
    q->head = newHead;
    if (newHead == NULL) {
        q->tail = NULL;
    }
    q->size--;
    return v;
}

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q) {
    return q->size == 0;
}
/**
 * Gets the size of the given queue
 */
int QueueSize(Queue q) {
    return q->size;
}
