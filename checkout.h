#ifndef checkout_h
#define checkout_h

#define MAX_SIZE 100
#define MAX_NAME_SIZE 50

//grocery item for each customer
typedef struct gItem
{
    int key;
    int amount;
} gItem;

//customer checkout detail
typedef struct customer_detail
{
    char *name;
    float cash;
    gItem grocery_list[MAX_SIZE];
    int no_of_items;
} customer_detail;

/* calculate the total pay amount of the customer 
 *
*/
float total(customer_detail customer);

// queue implementation
typedef struct queue
{
    customer_detail data;
    struct queue *next;
} queue;

/* enqueue the queue
 * takes new data in the form of customer_detail struct 
 * add the information to the rear of the queue
 * returns void
 * @params struct type customer_detail
 * @return void 
 */
void enqueue(customer_detail new_data);

/* dequeue the queue
 * takes no arguement and returns the customer info from front of the queue
 * also deletes the node from the front of the queue 
 * @params void
 * @returns struct type customer_detail 
*/
customer_detail dequeue(void);

#endif
