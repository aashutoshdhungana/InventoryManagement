#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100
//grocery item for each customer
typedef struct gItem
{
    int key;
    int amount;
} gItem;

//Checkout
typedef struct customer_detail
{
    char name[50];
    float cash;
    int (*grocery_list)[2]; //int grocery_list[MAX_SIZE][2];
} customer_detail;

typedef struct queue
{
    struct customer_detail data;
    struct queue *next;
} queue;

queue *rear = NULL, *front = NULL;

// to enqueue the queue
void enqueue(customer_detail *new_data);

// dequeue the queue
customer_detail *dequeue(void);

// calculate total for customer
float total(customer_detail customer)
{
}

//print the log
void print_log(customer_detail customer, bool has_enough_money)
{
    /* format :
        Customer - Karen

        Apples x3 @ $0.99
        Swiss Cheese x1 @ $2.49

        Total: $6.48
        Thank you, come back soon!
        (Customer did not have enough money and was REJECTED
        Thank you, come back soon!)-> if has_enough_money = FALSE;
        -------------------------------------------------------
    */
}

int main(void)
{
    FILE *fptr = NULL;
    char *customer = NULL;

    // input a file to read the queue??
    // here test.txt is useed
    fptr = fopen("test.txt", "r");

    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //repeat until eof
    //read file line by line
    //insert data into customer_detail struct
    //enqueue the data

    //dequeue the data untill rear
    //calculate total
    //total < cash :
    //print_log
    //total >= cash
    //deduct from inventory
    //check for if restock required
    //inventory message

    fclose(fptr);
    return 0;
}
