#include<stdio.h>
#include "inventory.c"
#include "checkout.c"
#include "inventory.h"
#include "helpers.c"
int main(void)
{
    inventorySystem(2);
    inventorySystem(3);
    atend();
}
