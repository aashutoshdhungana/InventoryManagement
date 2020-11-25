#include<stdio.h>
#include "inventory.c"
#include "checkout.c"
#include "inventory.h"
#include "helpers.c"
int main(void)
{
    load();
    // add menue to these
    if(!add(121, "hello wolrd", 15, 12, 12.0))
    {
        printf("Error 1 my friend\n");
    }

    if(!deleteItem(102))
    {
        printf("Error 2 my friend\n");
    }

    if(!restock(101, 5))
    {
        printf("Error 3 my friend\n");
    }

    if(!restockAll())
    {
        printf("Error 4 my friend\n");
    }
    updateTxt();
    atend();
    unload();
}
