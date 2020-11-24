#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "inventory.h"
#include "helpers.c"

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

bool search (int key)
{
    int hashKey = hash(key);
    node *temp = malloc(sizeof(node));
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

void updateTxt()
{
    //TODO
}

bool add(int key, string name, int threshold, int stock, float price)
{
    if (search(key))
    {
        printf("Key already exists\n");
        return 0;
    }
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
    if (search(key))
    {
        int hashkey = hash(key);
        node *temp = Table[hashkey];
        while (temp != NULL)
        {
            if (temp->items.key == key)
            {
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
        while (line[i] != '\n')// need to change parsing method
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
    string n =NULL;
    switch(option)
    {
        case 1:
            k = get_int("Enter the Key: ");
            string temp = get_string("Enter the name of item: ");
            n = realloc(temp, sizeof(char)*strlen(temp));
            t = get_int("Enter threshold of item: ");
            s = get_int("Enter stock of item: ");
            p = get_float("Enter price of item: ");

            if (add(k, n, t, s, p))
            {
                printf("success\n");
            }

            else
            {
                printf("\nError!!!!");
            }
            break;

        case 2:
            k = get_int("Enter the key of item to be deleted: ");
            if (deleteItem(k))
            {
                printf("Success!!!!\n");
            }

            else
            {
                printf("\nError!!!!");
            }
            break;

        case 3:
            k = get_int("Enter key of item to restock: ");
            int num = get_int("Enter the number of item to be added: ");
            if (restock(k, num))
            {
                printf("Success!!!!!!\n");
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
