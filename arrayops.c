#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "arrayops.h"

bool insert(int data)
{
    if (search_data(data))
    {
        // printf("Item already there!\n");
        return false;
    }
    node_array *nnode;
    nnode = (node_array *)malloc(sizeof(node_array));
    if (nnode == NULL)
    {
        printf("Memory allocation failed!!\n");
        exit;
    }
    nnode->data = data;
    nnode->next = head;
    head = nnode;
    return true;
}

bool remove_data(int data)
{
    node_array *temp, *freenode;
    temp = head;
    if (temp == NULL)
    {
        return false;
    }
    int pos = 0;
    while (temp != NULL)
    {
        if (temp->data == data)
        {
            if (pos == 0)
            {
                head = head->next;
                free(temp);
                return true;
            }
            temp = head;
            for (int i = 0; i < pos - 1; i++)
                temp = temp->next;

            freenode = temp->next;
            temp->next = temp->next->next;
            free(freenode);
            return true;
        }
        temp = temp->next;
        pos++;
    }
    return false;
}

bool search_data(int data)
{
    node_array *temp;
    temp = head;
    while (temp != NULL)
    {
        if (temp->data == data)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void deleteall()
{
    node_array *temp;
    temp = head;
    while (head != NULL)
    {
        head = head->next;
        free(temp);
        temp = head;
    }
}

int data_count()
{
    node_array *temp;
    int count = 0;
    temp = head;
    while (temp != NULL)
    {
        // printf("%d ", count);
        count++;
        temp = temp->next;
    }
    return count;
}

int get_data(int position)
{
    node_array *temp;
    temp = head;
    if (position >= data_count() || position < 0)
    {
        printf("Out of bound position!");
        exit(1);
    }
    for (int i = 0; i < position; i++)
    {
        temp = temp->next;
    }
    return temp->data;
}
