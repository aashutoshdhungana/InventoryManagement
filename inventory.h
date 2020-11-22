#ifndef INVENTORY_H
#define INVENTORY_H

#include<stdio.h>
#include<stdbool.h>

//might define string type later

typedef struct
{
    int key;
    char* name;
    int threshold;
    int stock;
    float price;
}
itemData;

typedef struct node
{
    itemData *items;
    struct node *next;
}
node;

//hash table functions
int hash(int key);
void load(itemData* item);
void search (int key);

//inventory management functions
bool add(int key, char *name, int threshold, int stock, float price);
bool deleteItem(int key);
bool restock();
bool restockAll();
#endif