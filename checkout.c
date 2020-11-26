#ifndef CHECKOUT_C
#define CHECKOUT_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "checkout.h"
#include "inventory.h"
#include "inventory.c"
#include "arrayops.h"
#include "arrayops.c"

#define MAX_LINE_SIZE 200
#define MAX_ITEM 200

int no_of_customers = 0;

//pointer to reference memory as customer.name to be freed later
char *ptr_to_free[MAX_ITEM];
static int ptr_to_free_index = 0;

//flag for restock
bool restock_alarm = false;

queue *rear = NULL, *front = NULL;

//make a receipt file
void make_receipt(char *filename);

//write receipt for each customer
void write_receipt(customer_detail customer, FILE *fptr);

//print inventory warning for restocking
void print_warning();

// helper functions
long extract_int(char *);

//get struct variable storing customer details
customer_detail make_customer(char[], float, int[], int);

//free pointer used to memory allocation of customer.name
void free_name_pointer_in_cd_struct();
gItem get_item_with_amount(int key);

//checkout register file
int checkout(char *filename)
{
    FILE *fptr = NULL;

    //open file
    fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    char *line;

    line = (char *)calloc(MAX_LINE_SIZE, sizeof(char));
    if (line == NULL)
    {
        printf("Memory allocation for line failed!!\n");
        exit(1);
    }

    int j;
    char *token, *str, name[MAX_NAME_SIZE];
    float cash;
    long num;

    //parse register file and enqueue customer
    while (fgets(line, MAX_LINE_SIZE, fptr))
    {
        int glist[MAX_ITEM], gindex = 0;
        for (j = 1, str = line;; j++, str = NULL)
        {
            /* line format eg: {"Karen", 8.00, [{102, 3}, {216, 1}]}
             * separate substrings with "," as delimeter
             */
            token = strtok(str, ",");

            //if no further substring break for loop
            if (token == NULL)
                break;

            //first extract name
            if (j == 1)
            {
                int index = 0;
                while (*token != '\0')
                {
                    if (isalpha(*token) || *token == ' ')
                        name[index++] = *token;
                    token++;
                }
                name[index] = '\0';
            }
            //second extract cash
            else if (j == 2)
            {
                cash = atof(token);
            }
            //extract numbers and place in 1D array for grocey list
            else
            {
                num = extract_int(token);
                glist[gindex++] = num;
            }
        }

        // get a customer with details
        customer_detail customer = make_customer(name, cash, glist, gindex / 2);

        // enqueue customer
        enqueue(customer);

        no_of_customers++;
    }

    free(line);
    free(str);

    /* call function at exit to free memory
     *`allocated to customer.name
     */
    atexit(free_name_pointer_in_cd_struct);

    //make receipt file
    make_receipt(filename);

    //print the inventory warning if necessary
    print_warning();

    fclose(fptr);
    return 0;
}

//extract integer from mixed string
long extract_int(char *ptr)
{
    long val = 0;
    char *endptr;
    while (*ptr != '\0' || *ptr)
    {
        // print_c(*ptr);
        if (isdigit(*ptr))
        {
            val = strtol(ptr, &endptr, 10);
            // printf("%ld\n", val);
            return val;
        }
        else
        {
            ptr++;
        }
    }
    return val;
}

// make variable of customer_detail struct
customer_detail make_customer(char name[], float cash, int grocery_list[], int nO_of_items_in_g_list)
{
    customer_detail customer;

    // allocate memory for customer.name
    customer.name = malloc((strlen(name) + 1) * sizeof(char));
    if (customer.name == NULL)
    {
        printf("Memory allocation failed!");
        exit(1);
    }

    //copy value of name to memory allocated
    strcpy(customer.name, name);

    //make a pointer to reference to the allocated memory
    //so that at the end of the program we could free it
    ptr_to_free[ptr_to_free_index++] = customer.name;

    customer.cash = cash;

    for (int i = 0, k = 0; i < nO_of_items_in_g_list; i++, k += 2)
    {
        customer.grocery_list[i].key = grocery_list[k];
        customer.grocery_list[i].amount = grocery_list[k + 1];
    }

    customer.no_of_items = nO_of_items_in_g_list;

    return customer;
}

void make_receipt(char *filename)
{
    // printf("%s", filename);
    customer_detail customer;

    //generate name for receipt file
    char *filename_cpy = malloc((strlen(filename)) * sizeof(char));
    strcpy(filename_cpy, filename);
    char *basename = strtok(filename_cpy, ".");
    strcat(basename, "_receipt.txt");

    FILE *fptr;

    // open file for writing
    fptr = fopen(basename, "w");
    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < no_of_customers; i++)
    {
        //dequeue customer from queue
        customer = dequeue();

        //write receipt for dequeued customer
        write_receipt(customer, fptr);
    }

    fclose(fptr);
    free(filename_cpy);
    filename_cpy = NULL;

    return;
}

void write_receipt(customer_detail customer, FILE *fptr)
{

    float total = 0, item_total = 0;
    int change_by;

    fprintf(fptr, "Customer - %s\n\n", customer.name);

    itemData itemDetail;

    // make pointer to the grocery_list array
    gItem *gList, *beginning;
    beginning = customer.grocery_list;
    gList = beginning;

    //go through grocery list one by one
    for (int i = 0; i < customer.no_of_items; i++)
    {
        //store item details(key, name, threshold, price etc) on itemDetails
        itemDetail = query(gList->key);

        //decrease the item in stock by amount
        change_by = -1 * gList->amount;
        restock(gList->key, change_by);

        //calculate total amount for the item
        item_total = gList->amount * itemDetail.price;

        fprintf(fptr, "%s X%d @ $%.2f\n", itemDetail.name, gList->amount, itemDetail.price);

        total += item_total;
        gList++;
    }

    fprintf(fptr, "\nTotal: $%.2f\n\n", total);

    //check if customer has enough cash, if not add items back to stock
    if (total <= customer.cash)
    {
        fprintf(fptr, "Thank you, come back soon!\n");
    }
    else
    {
        gList = beginning;
        for (int i = 0; i < customer.no_of_items; i++)
        {
            change_by = gList->amount;
            restock(gList->key, change_by);
            gList++;
        }
        fprintf(fptr, "Customer did not have enough money and was REJECTED\nThank you, come back soon!\n");
    }

    fprintf(fptr, "-------------------------------------------------------\n\n");

    //find items needing restocking
    gList = beginning;
    for (int i = 0; i < customer.no_of_items; i++)
    {
        itemDetail = query(gList->key);
        if (itemDetail.stock < itemDetail.threshold)
        {
            restock_alarm = true;
            insert(gList->key);
        }
        gList++;
    }
}

void print_warning()
{
    int restock_item, count;
    itemData item_detail;

    //get count of items needing restocking
    count = data_count();

    if (count != 0)
    {
        printf("\nWarning! The following Item(s) may need to be restocked:\n");
    }
    for (int k = 0; k < count; k++)
    {
        restock_item = get_data(k);
        item_detail = query(restock_item);
        printf("%d (%s): %d remain in stock, replenishment threshold is %d\n", item_detail.key, item_detail.name, item_detail.stock, item_detail.threshold);
    }

    deleteall();
}

// function to free all the allocated memory to customer.name
void free_name_pointer_in_cd_struct()
{

    for (int i = 0; i < no_of_customers; i++)
    {
        free(ptr_to_free[i]);
        ptr_to_free[i] = NULL;
    }
}

// enqueue customer detail
void enqueue(customer_detail new_data)
{
    queue *new_node;

    new_node = (queue *)malloc(sizeof(queue));
    if (new_node == NULL)
    {
        printf("Memory Allocation failed!!\n");
        exit(1);
    }

    if (rear == NULL)
    {
        new_node->data = new_data;
        new_node->next = NULL;
        front = rear = new_node;
    }
    else
    {
        new_node->data = new_data;
        new_node->next = NULL;
        rear->next = new_node;
        rear = new_node;
    }
}

// dequeue customer detail
customer_detail dequeue()
{
    queue *temp;
    customer_detail customer_info;

    if (front == NULL)
    {
        printf("Queue is empty! \n");
        exit(1);
    }
    else if (front->next == NULL)
    {
        temp = front;
        customer_info = temp->data;
        front = rear = NULL;
        free(temp);
    }
    else
    {
        temp = front;
        customer_info = temp->data;
        front = front->next;
        free(temp);
    }

    return customer_info;
}

#endif