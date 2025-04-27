#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "inventory.h"

// Global inventory array and counter
Item inventory[MAX_ITEMS];
int numItems = 0;

// Mutex to protect inventory access
pthread_mutex_t inventoryLock = PTHREAD_MUTEX_INITIALIZER;

// Threshold for triggering restock
#define RESTOCK_THRESHOLD 3
#define RESTOCK_AMOUNT 10

// Initialize the inventory with sample items
void initializeInventory() {
    pthread_mutex_lock(&inventoryLock);

    inventory[0] = (Item){.id = 1, .name = "Shampoo", .quantity = 5, .price = 5.99};
    inventory[1] = (Item){.id = 2, .name = "Soap", .quantity = 4, .price = 1.49};
    inventory[2] = (Item){.id = 3, .name = "Toothpaste", .quantity = 6, .price = 3.49};
    numItems = 3;

    pthread_mutex_unlock(&inventoryLock);
}

// Restock a specific item by ID
void restockItem(int itemId) 
{
    pthread_mutex_lock(&inventoryLock);

    for (int i = 0; i < numItems; i++) 
    {
        if (inventory[i].id == itemId) 
        {
            int restockAmount = 10 - inventory[i].quantity; // calculate how much needed to reach 10
            inventory[i].quantity = 10; // set quantity to 10

            printf("[Restock] Item %s (ID %d) restocked by %d units. New quantity: %d\n",
                   inventory[i].name, itemId, restockAmount, inventory[i].quantity);
            break;
        }
    }

    pthread_mutex_unlock(&inventoryLock);
}


// Utility to get item index in inventory by ID
int getItemIndexById(int itemId) {
    for (int i = 0; i < numItems; i++) {
        if (inventory[i].id == itemId) {
            return i;
        }
    }
    return -1; // not found
}