// COMP2521 Assignment 1

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invertedIndex.h"
#include "helper.h"


// Part 1

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    //open collection file name file
    FILE *collectionPointer = fopen(collectionFilename, "r");
    if (collectionPointer == NULL) {
		fprintf(stderr, "error: failed to open %s\n", collectionFilename);
		exit(EXIT_FAILURE);
    }
    char file[MAX_WORD + 1];
    //variable to store words
    char operatedWord[MAX_WORD + 1];
    char rawWord[MAX_WORD + 1];
    //empty tree
    InvertedIndexBST root = NULL;
    
    //read line in collectionFilename
    while (fscanf(collectionPointer, "%s", file) == 1) {
    
        //word count for the file for 'tf' calculation
        int fileWordCount = 0;


        //open file from file collection
        FILE *filesPointer = fopen(file, "r");
        if (filesPointer == NULL) {
		    fprintf(stderr, "error: failed to open %s\n", file);
		    exit(EXIT_FAILURE);
        }
        
        //iterate through the text file word by word
        while (fscanf(filesPointer, "%s" ,rawWord) == 1) {
            
            //store normalized word as operatedWord
            strcpy(operatedWord, wordsOPeration(rawWord));
            //skip empty words
            if (operatedWord[0] == 0 || strlen(operatedWord) == 0) {
                continue;
            }
            fileWordCount++;
            if (root == NULL) {
                root = createWholeNode(operatedWord, file);
                
            }
            else {
                //case 1: new word --> create new node and insert the node to the tree
                //case 2: same word --> case 2a : have file node --> increament tf value by 1
                //                  --> case 2b : doesn't have repective file node --> create and initialize file node                

                //indicator for weather the word is in tree
                int IsInTree = 0;
                
                //search for the word
                
                IsInTree = haveWord(root, operatedWord);
                
                
                //case 1 (new word)
                if (IsInTree == 1) {
                    insertNode(root, operatedWord, file);
                    //printf("not found\n");
                }
                //case 2 (same word)
                //search for the file node
                else if (IsInTree == 2) {
                    FileNodeOPeration(root, file, operatedWord);
                    
                }
            }
        }
        //divide all tf values with the file word count
        UpdateTF(root, file, fileWordCount);
        //close files
        fclose(filesPointer);
    }
    //close collection
    fclose(collectionPointer);
    //return generated inverted index BST
	return root;
}


//format: 
//        ascribed file11.txt (0.0303030) file21.txt (0.0625000)
void printInvertedIndex(InvertedIndexBST tree, char *filename) {

    FILE *out = fopen(filename, "w+");
    if (out == NULL) {
		fprintf(stderr, "error: failed to open %s\n", filename);
		exit(EXIT_FAILURE);
    }
    //printing
    doPrintInvertedIndex(tree, filename, out);
 
    fclose(out);
}

void freeInvertedIndex(InvertedIndexBST tree) {
    //free everynode in the tree
    bool freeList = false;
    doTreeFree(tree, freeList);
}


// Part 2
//tf-idf = tf * idf
//idf = log10(total number of document / number of document containing the word)
TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D) {
    TfIdfList result = NULL;
    //find the target word node
    InvertedIndexBST target = findTargetNode(tree, searchWord);
    
    FileList current = target->fileList;
    
    //find number of document containing the word (i.e length of the target fileList)
    int numberOfDoc = findLength(current);
    while (current->next != NULL) {
        //first node to insert
        if (result == NULL) {
            //create first node of TfIdfList
            result = createTfIdfList(current, D, numberOfDoc);
            current = current->next;
        }
        else {
            //insert nodes in order
            result = InsertTfIdfListInorder(result, current, D, numberOfDoc);
            current = current->next;
        }
    }
    //insert the last node of the list
    result = InsertTfIdfListInorder(result, current, D, numberOfDoc);
    //return ordered TfIdfList
	return result;
}

TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D) {
    //variable for iterating the searchWords array
    int i = 0;
    TfIdfList result = NULL;
    
    while (searchWords[i] != NULL) {
        InvertedIndexBST target = findTargetNode(tree, searchWords[i]);
        FileList current = target->fileList;
        //find number of document containing the word (i.e length of the target fileList)
        int numberOfDoc = findLength(current);
        //insert node into result list
        //if node already exist update the tf-idf
        while (current->next != NULL) {
            if (result == NULL) {
                //create first node 
                result = createTfIdfList(current, D, numberOfDoc);
                current = current->next;
            }
            else {
                result = TfIdfListOperation(result, current, D, numberOfDoc);
                current = current->next;
            }
        }
        result = TfIdfListOperation(result, current, D, numberOfDoc);
        i++;
    }
    //sort the list in ascending order
    sortResult(result);
	return result;
}

void freeTfIdfList(TfIdfList list) {

}

