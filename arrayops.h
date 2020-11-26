#ifndef arrayops_h
#define arrayops_h

#include <stdio.h>
#include <stdbool.h>

typedef struct node_array
{
    int data;
    struct node_array *next;
} node_array;

bool insert(int data);
bool remove_data(int data);
bool search_data(int data);
void deleteall();
int data_count();
int get_data(int);
#endif