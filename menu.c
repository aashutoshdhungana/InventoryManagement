#include<stdio.h>
#include "inventory.c"
#include "checkout.c"
#include "inventory.h"
#include "helpers.c"
int main(void)
{
    load();
    updateTxt();
    atend();
    unload();
}
