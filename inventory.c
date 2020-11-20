#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define TABLESIZE 1000 
// Type defination for string data structure
typedef char* string;

// Item type defination
typedef struct
{
    int key;
    string name;
    int threshold;
    int stock;
    float price;
}
itemData;

// Node of Hash Table
typedef struct node
{
    itemData items;
    struct node *next;
}
node;

// HashTable declaration
node *Table[TABLESIZE];

bool add()
{
    //TODO
    return 0;
}

bool deleteItem()
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

    //read and insert to a hash table we'll discuss

    switch(option)
    {
        case 1:
            if (add())
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

            if (deleteItem())
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
}
