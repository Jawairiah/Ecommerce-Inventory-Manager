#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_ITEMS 100

typedef struct {
    int id;
    char name[50];
    int quantity; 
    float price;
} Item;

extern Item inventory[MAX_ITEMS];
extern int numItems;
extern pthread_mutex_t inventoryLock;


int getItemIndexById(int itemId);
void initializeInventory();
void restockItem(int itemId);

#endif
