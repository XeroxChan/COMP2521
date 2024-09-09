// Dijkstra API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

//helper function declaration
PredNode *make_pred_node(int current);
PredNode *insert_pred_node(NodeData *result, int current, int target);


NodeData *dijkstra(Graph g, Vertex src) {
    //initialse memory for array of NodeData with length of number of vertices in g
    NodeData *result = malloc(GraphNumVertices(g) * sizeof(NodeData));
    if (result == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    //array to mark if a vertex is visited
    int visited[GraphNumVertices(g)];
    for (int j = 0; j < GraphNumVertices(g); j++) {
        visited[j] = -1;
    }
    
    //create new priority queue
    PQ priority_queue = PQNew();
    
    //initialize result array
    for (int i = 0; i < GraphNumVertices(g); i++) {
        if (i != src) {
            result[i].dist = INFINITY;
            result[i].pred = NULL;
        }
        else {
             //initialse src data
            result[src].dist = 0;
            result[src].pred = NULL;
        }
    }
    

    
    //add strating vertex (src) to the priority_queue
    PQInsert(priority_queue, src, result[src].dist);
    
    
    //loop until the priority_queue is empty
    while (!PQIsEmpty(priority_queue)) {
    
        //get the vertex with the highest priority from priority_queue (lowest dist)
        int current = PQDequeue(priority_queue);
        
        //mark as visited
        visited[current] = 1;

        
        //get a list of out edges of the current vertex
        AdjList out_edges = GraphOutIncident(g, current);
        AdjList out_list = out_edges;
        
        //go through all the out edges from the vertex
        while (out_list != NULL) {

            //if shorter path to the vertex through current, update the distance and replace the predecessor array
            if (result[current].dist + out_list->weight < result[out_list->v].dist) {
                                                                                            
                //update distance of the vertex in the result list 
                result[out_list->v].dist = result[current].dist + out_list->weight;

                //make a pred_node
                //current_pred_node->v = current
                //current_pred_node->next = NULL
                //  as this is the shortest path and should have only one PredNode
                PredNode *current_pred_node = make_pred_node(current);
                
                //replace predecessor linked list with the current_pred_node
                result[out_list->v].pred = current_pred_node;
                
                
            }
            
            //if same distance
            //distance no need to be updated
            //add current to predecessor array in order
            else if (result[current].dist + out_list->weight == result[out_list->v].dist) {

                //add the new pred to the pred list in order
                result[out_list->v].pred = insert_pred_node(result, current, out_list->v);
                
            }
            
            //insert the out_edges to the priority_queue if not visited
            if (visited[out_list->v] == -1) {
                PQInsert(priority_queue, out_list->v, result[out_list->v].dist);
            }


            out_list = out_list->next;
        }
        
    }
	return result;
}

void freeNodeData(NodeData *data, int nV) {
    
    for (int i = 0; i < nV; i++) {
        PredNode *tmp;
        PredNode *head = data[i].pred;
        while (head != NULL) {
            tmp = head;
            head = head->next;
            free(tmp);
        }
        free(&data[i].dist);
    }
    free(data);
}


//helper function

//make a new PredNode
PredNode *make_pred_node(int current) {
    //malloc memory for the new PredNode
    PredNode *new = malloc(sizeof(PredNode));
    //initialize new 
    new->v = current;
    new->next = NULL;
    return new;
}

//insert pred node
PredNode *insert_pred_node(NodeData *result, int current, int target) {
    PredNode *pred_node_to_add = make_pred_node(current);
    PredNode *head = result[target].pred;
    PredNode *tmp = result[target].pred;

    //insert at head
    if (pred_node_to_add->v < head->v) {
        //the pred list points to the new node
        result[target].pred = pred_node_to_add;
        //new pred node points to original head
        pred_node_to_add->next = head;
    }
    
    //iterate through the pred list to find the correct spot to insert
    else {
        while (tmp->next != NULL) {
            //if node to add is larger than next node, go to next node
            if (pred_node_to_add->v > tmp->next->v) {
                tmp = tmp->next;
            }
            
            //insert node in betweeen
            else if (pred_node_to_add->v < tmp->next->v) {
                pred_node_to_add->next = tmp->next;
                tmp->next = pred_node_to_add;
                break;
            }
        }
        //add to the end of the pred list
        if (tmp->next == NULL) {
            tmp->next = pred_node_to_add;
        }
    }
    return result[target].pred;
}
