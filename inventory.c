#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "inventory.h"
#include "helpers.c"

#define MAX_WORD 45
#define MAX_LINE 45*5

// Dynamic list of keys to store all the keys in inventory
int *keys = NULL;
int numofkeys = 0;

//final build needs to have a better hash function this woks for now
int hash(int key)
{
    return key%1000;
}

// Function to load the hash table
void load()
{
    // Opening file
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL)
    {
        printf("Inventory.txt couldnot be opened!!!!!\n");
        exit(1);
    }

    // Reading from file then parsing the read data
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file))
    {
        int i = 0;
        int j = 0;
        int k = 0;
        char words[4][MAX_WORD];
        while (line[i] != '\0')
        {
            if (line[i] == ',')
            {
                words[j][k] = '\0';
                j++;
                i = i + 2;
                k = 0;
            }

            else if (line[i] == '"' || line[i] == '{')
            {
                i++;
            }

            else if (line[i] == '}')
            {
                words[j][k] = '\0';
                break;
            }
            else
            {
                words[j][k] = line[i];
                k++;
                i++;
            }
        }
        if(!add(atoi(words[0]), words[1], atoi(words[2]), atoi(words[3]), atof(words[4])))
        {
            printf("Loading failed!!\n");
            exit(-1);
        }
    }
    fclose(file);
    return;
}

bool search (int key)
{
    int hashKey = hash(key);
    node *temp;
    temp = Table[hashKey];
    while (temp != NULL)
    {
        if (temp->items.key == key)
        {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

// Updates the text file at the end of the program
void updateTxt()
{
    FILE * file = fopen("inventory.txt", "w");
    for (int i = 0; i < numofkeys; i++)
    {
        int hashkey = hash(keys[i]);
        node *n = Table[hashkey];
        while( n != NULL)
        {
            fprintf(file, "{%.3i, \"%s\", %i, %i, %.2f}\n", n->items.key, n->items.name, n->items.threshold, n->items.stock, n->items.price);
            n = n->next;
        }
    }
    fclose(file);
}

// Adds into hash table
bool add(int key, string name, int threshold, int stock, float price)
{
    if (search(key))
    {
        printf("Key already exists\n");
        return 0;
    }

    // Data assigned to item;
    itemData item;
    item.key = key;
    item.name = malloc(sizeof(char) * (strlen(name) + 1));
    if (item.name == NULL)
    {
        printf("Memory Allocation Failed\n");
    }
    strcpy(item.name, name);
    item.threshold = threshold;
    item.stock = stock;
    item.price = price;

    // Adding item to hash table
    int hashkey = hash(item.key);
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        printf("Memory Allocation Failed!!\n");
        return 0;
    }
    n->items = item;
    n->next = Table[hashkey];
    Table[hashkey] = n;

    // Add keys in list
    int *temp = realloc(keys, sizeof(int)*(numofkeys + 1));
    keys = temp;
    keys[numofkeys] = item.key;
    numofkeys ++;
    return 1;
}

// Deletes item from hash table
bool deleteItem(int key)
{
    if (search(key))
    {
        int hashkey = hash(key);
        node *temp = Table[hashkey];
        while (temp != NULL)
        {
            if (temp->items.key == key)
            {
                free(temp->items.name);
                node *temp1 = temp->next;
                while (temp1 != NULL)
                {
                    if (temp1->next == NULL)
                    {
                        temp1->next = Table[hashkey];
                        break;
                    }
                    temp1 = temp1->next;
                }
                Table[hashkey] = temp->next;
                free(temp);
                break;
            }
            temp = temp->next;
        }
        return 1;
    }
    printf("key not found\n");
    return 0;
}

// Function to restock the 'key' item by adding 'num' amount
bool restock(int key, int num)
{
    if (search(key))
    {
        int hashkey = hash(key);
        node *temp = Table[hashkey];
        while (temp != NULL)
        {
            if (temp->items.key == key)
            {
                temp->items.stock += num;
                return 1;
            }
            temp = temp->next;
        }
        return 1;
    }
    printf("Key not found\n");
    return 0;
}

// Restocks all the items to threshold value
bool restockAll() 
{
    for (int i = 0; i < numofkeys; i++)
    {
        int hashkey = hash(keys[i]);
        node *n = Table[hashkey];

        while (n != NULL)
        {
            if (n->items.key == keys[i])
            {
                if (n->items.stock < n->items.threshold)
                {
                    n->items.stock = n->items.threshold;
                }
            }
            n = n->next;
        }
    }
    return 1;
}

// Function frees all the memories
void unload()
{
    for(int i = 0; i < numofkeys; i++)
    {
        if (search(keys[i]))
        {
            deleteItem(keys[i]);
        }
    }
    free(keys);
}