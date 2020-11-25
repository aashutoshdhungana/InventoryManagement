#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "checkout.h"

#define MAX_LINE_SIZE 200
#define MAX_ITEM 200

#define print_c(c) printf("%c\n", c)
#define print_int(msg, i) printf("%s %d\n", msg, i)
#define print_f(f) printf("float = %f\n", f)

int no_of_customers = 0;

//pointer to reference memory as customer.name to be freed later
char *ptr_to_free[MAX_ITEM];
static int ptr_to_free_index = 0;

queue *rear = NULL, *front = NULL;

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

// helper functions
long extract_int(char *);
customer_detail make_customer(char[], float, int[], int);
void free_name_pointer_in_cd_struct();

// int checkout(char *filename)
int main(void)
{
    // inventorySystem(9);
    printf("Loaded hash table.\n");

    FILE *fptr = NULL;
    // char *customer = NULL;

    // input a file to read the queue??
    // here test.txt is useed
    fptr = fopen("test.txt", "r");

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
            //extract numbers and place in 2D array
            else
            {
                num = extract_int(token);
                glist[gindex++] = num;
            }
        }

        // fill in struct
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

    customer_detail customer;
    for (int i = 0; i < no_of_customers; i++)
    {
        customer = dequeue();
        printf("Dequeued customer is %s\n", customer.name);
    }

    // dequeue one item
    //repeat until eof - O
    //read file line by line - O
    //insert data into customer_detail struct - O
    //enqueue the data - O

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

    int n = strlen(name) + 1; // +1 for '\0'

    // allocate memory for customer.name
    customer.name = malloc(n * sizeof(char));
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

    /* store grocery list in two dimentional array
     * glist[no_of_items][2]
     */
    int k = 0;
    for (int i = 0; i < nO_of_items_in_g_list; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            customer.grocery_list[i][j] = grocery_list[k++];
        }
    }

    customer.no_of_items = nO_of_items_in_g_list;

    return customer;
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
    // printf("%s, %f, {%d %d}\n", new_node->data.name, new_node->data.cash, new_node->data.grocery_list[0][0], new_node->data.grocery_list[0][1]);
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