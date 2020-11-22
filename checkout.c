#include <stdio.h>
#include <stdlib.h>

//grocery item for each customer
typedef struct gItem
{
    int key;
    int amount;
} gItem;

//Checkout
typedef struct checkout
{
    char name[100];
    float cash;
    int (*grocery_list)[2];
} checkout;

typedef struct queue
{
    struct checkout customer_data;
    struct queue *next;
} queue;

int main(void)
{
    FILE *fptr = NULL;
    char *customer = NULL;

    fptr = fopen("test.txt", "r");

    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //loop throuh file
    //read file line by line

    fclose(fptr);
    return 0;
}
