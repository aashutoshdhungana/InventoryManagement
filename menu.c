#include <stdio.h>
#include <stdlib.h>
#include "inventory.c"
#include "checkout.c"
#include "helpers.c"

void main_menu();
void print_main_menu();
void print_inventory_submenu();

int main(void)
{
    load();
    main_menu();
    updateTxt();
    atend();
    unload();
}

void main_menu()
{
    int option;
    print_main_menu();
    option = get_int("Enter an option: ");

    while (true)
    {
        if (option == 1 || option == 2 || option == 3)
            break;
        else
            option = get_int("Enter an option: ");
    }

    switch (option)
    {
    case 1:
        system("CLS");
        printf("\nMenu > Inventory\n");
        printf("--------------------------------------------------------\n\n");

        // int inventory_option;
        print_inventory_submenu();
        break;
    case 2:
        system("CLS");
        printf("\nMenu > Checkout\n");
        printf("--------------------------------------------------------\n\n");
        string filename;
        filename = get_string("Enter a file name: ");
        checkout(filename);
        break;
    case 3:
        break;
    default:
        break;
    }
}

void print_main_menu()
{
    system("CLS");
    printf("\n------------ Inventory Management System ------------\n\n\n");
    printf("1. Check-out Queue of Customers\n");
    printf("2. Manage Inventory\n");
    printf("3. Close Program\n\n");
    printf("--------------------------------------------------------\n\n");
}

void print_inventory_submenu()
{
    printf("1. Add items\n");
    printf("2. Delete items\n");
    printf("3. Restock an item\n");
    printf("4. Restock all items under threshold\n");
}
