#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
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

bool add(int key, string name, int threshold, int stock, float price)
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
        printf("%s, %s, %s, %s, %s\n", words[0], words[1], words[2], words[3], words[4]);
    }
    //read and insert to a hash table we'll discuss
    int k, t, s;
    float p;
    string n;
    switch(option)
    {
        case 1:
            printf("Key\t\t|\t");
            scanf("%i", &k);
            printf("Name\t\t|\t");
            scanf("%s", &n);
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
