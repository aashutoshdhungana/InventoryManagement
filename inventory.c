#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "inventory.h"
#define TABLESIZE 1000

// Type defination for string data structure
typedef char* string;

// HashTable declaration
node *Table[TABLESIZE];

//final build needs to have a better hash function this woks for now
int hash(int key)
{
    return key%1000;
}

void load(itemData item)
{
    int hashKey = hash(item.key);
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        printf("Mem Error!!!");
        exit(1);
    }
    n->items = item;
    n->next =  Table[hashKey];
    Table[hashKey] = n;
    return;
}

void search (int key)
{
    int hashKey = hash(key);
    node *temp = malloc(sizeof(node));
    temp = Table[hashKey];
    while (temp != NULL)
    {
        if (temp->items.key == key)
        {
            printf("found\n");
            return;
        }
        temp = temp->next;
    }
    printf("Not found\n");
    return;
}

void updateTxt()
{
    //TODO
}

bool add(int key, string name, int threshold, int stock, float price)
{
    itemData item;
    item.key = key;
    item.name = malloc(sizeof(char)*strlen(name));
    strcpy(item.name, name);
    item.threshold = threshold;
    item.stock = stock;
    item.price = price;
    load(item);
    return 1;
}

bool deleteItem(int key)
{
    //TODO
    return 0;
}

bool restock()
{
    //TODO
    return 0;
}

bool restockAll()
{
    //TODO
    return 0;
}

void inventorySystem(int option)
{
    // Opening file
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL)
    {
        printf("Inventory.txt couldnot be opened!!!!!\n");
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        int i = 0;
        char words[5][15];
        int j = 0;
        int k =0;
        while (line[i] != '\n')
        {
            if (line[i] == '{' || line[i] == ' ' || line[i] == '"')
            {
            }

            else if (line[i] == '}')
            {
                words[j][k] = '\0';
            }

            else if (line[i] == ',')
            {
                words[j][k] = '\0';
                k = 0;
                j++;
            }

            else
            {
                words[j][k] = line[i];
                k++;
            }
            i++;
        }
        itemData item;
        item.key = atoi(words[0]);
        item.name = malloc(sizeof(char)*strlen(words[1]));
        strcpy(item.name, words[1]);
        item.threshold = atoi(words[2]);
        item.stock = atoi(words[3]);
        item.price = atof(words[4]);
        load(item);
    }
    //read and insert to a hash table we'll discuss
    int k, t, s;
    float p;
    char n[20];
    switch(option)
    {
        case 1:
            printf("Key\t\t|\t");
            scanf("%i", &k);
            printf("Name\t\t|\t");
            scanf("%s", n);
            printf("Threshold\t|\t");
            scanf("%i", &t);
            printf("Stock\t\t|\t");
            scanf("%i", &s);
            printf("Price\t\t|\t");
            scanf("%f", &p);

            if (add(k, n, t, s, p))
            {
                printf("\nsuccess");
            }

            else
            {
                printf("\nError!!!!");
            }
            break;

        case 2:
            //TODO check if key exist

            //

            if (deleteItem(k))
            {
                printf("\nSuccess!!!!");
            }

            else
            {
                printf("\nError!!!!");
            }
            break;

        case 3:
            if (restock())
            {
                printf("\nSuccess!!!!");
            }

            else
            {
                printf("\nError!!!!");
            }
            break;

        case 4:
            if (restockAll())
            {
                printf("\nSuccess!!!!");
            }

            else
            {
                printf("\nError!!!!");
            }
            break;

        default:
            printf("Enter valid option");
            break;
    }
    fclose(file);
    return;
}
