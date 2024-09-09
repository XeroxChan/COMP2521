#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdbool.h>
#include "invertedIndex.h"
#define MAX_WORD 100


//words operation - upper to lower and normalization
//return operated string
char *wordsOPeration(char rawWord[]);

//use for checking punctuations
int isTargetPunc(char character);

//creation of nodes

//createWholeNode
InvertedIndexBST createWholeNode(char operatedWord[], char *file);
//create tree node
InvertedIndexBST createTreeNode(char operatedWord[]);
//create File node
FileList createListNode(char *file);
//create TfIdfList
TfIdfList createTfIdfList(FileList current, int D, int numberOfDoc);


//insertion of nodes

//insert node to tree
InvertedIndexBST insertNode(InvertedIndexBST root, char operatedWord[], char *file);
//insert file node in order
void InsertFileInOrder(InvertedIndexBST target,char *file);
//insert tfidf node to list
TfIdfList InsertTfIdfListInorder(TfIdfList result, FileList current, int D, int numberOfDoc);



//see if word is in tree
int haveWord(InvertedIndexBST root, char operatedWord[]);


//update tf value if file node is present
//create file node otherwise
void FileNodeOPeration(InvertedIndexBST root, char *file, char operatedWord[]);


//go to the target node
InvertedIndexBST findTargetNode(InvertedIndexBST root, char operatedWord[]);

//divide all tf values with the file word count
void UpdateTF(InvertedIndexBST root, char *file, int fileWordCount);

//print tree
void doPrintInvertedIndex(InvertedIndexBST tree, char *filename, FILE *out);

//find length of file list
int findLength(FileList current);

//calculate Tf-Idf
double calculateTfIdf(double tf, double D, double numberOfDoc);


//this function decides to insert node or to update TfIdf value
TfIdfList TfIdfListOperation(TfIdfList result, FileList current_file_node, int D, int numberOfDoc);

//check if the node already exists
//return 0 if doesnt
//return 1 if exist
int findTfIdfNode(TfIdfList result, FileList current_file_node);

//update tfidf value
TfIdfList updateTfIdf(TfIdfList result, FileList current_file_node, int D, int numberOfDoc);

//sort TfIdfList according to descending tfIdf
void sortResult(TfIdfList result);

//swap datas between a and b
void swapData(TfIdfList a, TfIdfList b);


//free every node in the tree recursively
void doTreeFree(InvertedIndexBST tree, bool freeList);

//free every node in list
void listFree(FileList list);

#endif
