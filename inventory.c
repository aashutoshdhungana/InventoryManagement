#include<stdio.h>
#include<stdlib.h>
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
item;

void inventorySystem(int option)
{
    // Opening file
    File *file = fopen("inventory.txt", "r");

    //read and insert to a hash table we'll discuss

    switch(option)
    {
        case 1:
            int k, t, s, p;
            string n;
            printf("\nEnter key, name, threshold, stock and price of item");
            scanf("%i, %s, %i, %i, %f",&k, &n, &t, &s, &p);

            //TODO check if key is unique
            //

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
            int k;
            printf("\n Enter the key");
            scanf("%i", &k);

            //TODO check if key exist

            //

            if (delete(k))
            {
                printf("\nSuccess!!!!");
            }

            else
            {
                printf("\nError!!!!");
            }
            break;

        case 3:
            int k, num;
            printf("\n Enter the key and num");
            scanf("%i, %i", &k, &num);

            //TODO check if key exist and num fills to threshold

            //

            if (restock(k, num))
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

bool add(int key, string name, int threshold, int stock, float price)
{
    //TODO
    return 0;
}

bool deleteItem(int key)
{
    //TODO
    return 0;
}

bool restock(int key, int num)
{
    //TODO
    return 0;
}

bool restockAll()
{
    //TODO
    return 0;
}
