#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

void* customerThread(void* arg);
void* inventoryManagerThread(void* arg);
void placeOrder(int customerId, int itemId, int quantity);

void initializeThreads();
void cleanupResources();

#endif
