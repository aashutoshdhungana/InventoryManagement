#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "checkout.h"
//  #include "inventory.h"
// #include "inventory.c"
#include "arrayops.h"
#include "arrayops.c"

#define MAX_LINE_SIZE 200
#define MAX_ITEM 200

int no_of_customers = 0;
int restok_item[MAX_SIZE];
//pointer to reference memory as customer.name to be freed later
char *ptr_to_free[MAX_ITEM];
static int ptr_to_free_index = 0;

queue *rear = NULL, *front = NULL;

void write_receipt(customer_detail customer, gItem *list, FILE *fptr);
void make_receipt(char *filename);
void print_warning();

// helper functions
long extract_int(char *);
customer_detail make_customer(char[], float, int[], int);
void free_name_pointer_in_cd_struct();
gItem get_item_with_amount(int key);

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

    make_receipt(filename);
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

gItem get_item_with_amount(int key)
{
    int hashKey = hash(key);
    gItem item;
    node *temp;
    temp = Table[hashKey];
    while (temp->items.key != key)
    {
        printf("%d %d", temp->items.key, key);
        temp = temp->next;
    }
    item.key = key;
    item.price = temp->items.price;
    item.stock = temp->items.stock;
    item.threshold = temp->items.threshold;
    strcpy(item.name, temp->items.name);
    return item;
}

void make_receipt(char *filename)
{
    // printf("%s", filename);
    customer_detail customer;

    char *filename_cpy = malloc((strlen(filename)) * sizeof(char));
    strcpy(filename_cpy, filename);
    char *basename = strtok(filename_cpy, ".");
    strcat(basename, "_receipt.txt");

    FILE *fptr;
    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fptr = fopen(basename, "w");
    for (int i = 0; i < no_of_customers; i++)
    {

        customer = dequeue();
        // printf("Dequeued customer is %s\n", customer.name);

        gItem items[customer.no_of_items];

        for (int j = 0; j < customer.no_of_items; j++)
        {
            // printf("grocery key = %d\n", customer.grocery_list[j][0]);
            if (search(customer.grocery_list[j][0]))
            {
                items[j] = get_item_with_amount(customer.grocery_list[j][0]);
                items[j].amount = customer.grocery_list[j][1];
            }
        }

        write_receipt(customer, items, fptr);
    }

    fclose(fptr);
    free(basename);
    basename = NULL;
    // free(filename_cpy);
    return;
}

void write_receipt(customer_detail customer, gItem *list, FILE *fptr)
{
    gItem *beginning = list;

    float total = 0, item_total = 0;
    int change;

    fprintf(fptr, "Customer - %s\n\n", customer.name);
    // printf("Customer - %s\n\n", customer.name);

    for (int i = 0; i < customer.no_of_items; i++)
    {
        change = -1 * list->amount;
        // printf("change = %d\n", change);
        // printf("beforestock = %d\n", list->stock);
        restock(list->key, change);
        list->stock = list->stock + change;
        // printf("afterstock = %d\n", list->stock);
        item_total = list->amount * list->price;
        fprintf(fptr, "%s X%d @ $%.2f\n", list->name, list->amount, list->price);
        //checkfor stock threshhold
        // printf("**%s :  %d < %d\n", list->name, list->stock, list->threshold);
        total += item_total;
        list++;
    }

    fprintf(fptr, "\nTotal: $%.2f\n\n", total);
    if (total <= customer.cash)
    {
        fprintf(fptr, "Thank you, come back soon!\n");
    }
    else
    {
        list = beginning;
        for (int i = 0; i < customer.no_of_items; i++)
        {
            change = list->amount;
            restock(list->key, change);
            list->stock = list->stock + change;
            list++;
        }
        fprintf(fptr, "Customer did not have enough money and was REJECTED\nThank you, come back soon!\n");
    }

    fprintf(fptr, "-------------------------------------------------------\n\n");

    //find items for inventory warning
    list = beginning;
    for (int i = 0; i < customer.no_of_items; i++)
    {
        if (list->stock < list->threshold)
        {
            insert(list->key);
            // printf("\n\nInserted Item  = %d \n", list->key);
        }
        list++;
    }
}

void print_warning()
{
    int restock_item, count;
    count = data_count();
    if (count != 0)
    {
        printf("\nWarning! The following Item(s) may need to be restocked:\n");
    }
    for (int k = 0; k < count; k++)
    {
        restock_item = get_data(k);
        gItem item;
        item = get_item_with_amount(restock_item);
        printf("%d (%s): %d remain in stock, replenishment threshold is %d\n", item.key, item.name, item.stock, item.threshold);
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
