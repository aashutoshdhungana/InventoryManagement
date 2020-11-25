#ifndef INVENTORY_H
#define INVENTORY_H

#include<stdio.h>
#include<stdbool.h>
#define TABLESIZE 1000

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
    itemData items;
    struct node *next;
}
node;

// HashTable declaration
node *Table[TABLESIZE];

//hash table functions
int hash(int key);
void load();
bool search (int key);
void unload();
void updateTxt();

//inventory management functions
bool add(int key, char *name, int threshold, int stock, float price);
bool deleteItem(int key);
bool restock(int key, int num);
bool restockAll();
#endif