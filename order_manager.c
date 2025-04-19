#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "inventory.h"
#include "order_manager.h"
#include "logger.h"

// Number of customer threads
#define NUM_CUSTOMERS 5

// Global semaphore to signal restocking
sem_t stockThreshold;

// Array to store thread IDs
pthread_t customerThreads[NUM_CUSTOMERS];
pthread_t managerThread;

// Mutex (declared in inventory.c) is used to protect shared inventory

void* customerThread(void* arg) {
    int customerId = *(int*)arg;
    free(arg);

    // Simulate multiple orders
    for (int i = 0; i < 3; i++) {
        int itemId = (rand() % numItems) + 1; // Random item ID
        int quantity = (rand() % 4) + 1;      // Quantity between 1-4

        placeOrder(customerId, itemId, quantity);
        sleep(rand() % 2); 
    }

    return NULL;
}

// Order placement logic
void placeOrder(int customerId, int itemId, int quantity) {
    pthread_mutex_lock(&inventoryLock);

    int index = getItemIndexById(itemId);
    if (index == -1) {
        printf("Item ID %d not found.\n", itemId);
        pthread_mutex_unlock(&inventoryLock);
        return;
    }

    if (inventory[index].quantity >= quantity) {
        inventory[index].quantity -= quantity;

        printf("[Order] Customer %d ordered %d of %s (ID %d). Remaining: %d\n",
               customerId, quantity, inventory[index].name, itemId, inventory[index].quantity);

        logOrder(customerId, itemId, quantity, 1); // success

        if (inventory[index].quantity < 3) {
            sem_post(&stockThreshold); // trigger restock
        }
    } else {
        printf("[Order] Customer %d failed to order %d of %s (only %d left).\n",
               customerId, quantity, inventory[index].name, inventory[index].quantity);

        logOrder(customerId, itemId, quantity, 0); // failure
    }

    pthread_mutex_unlock(&inventoryLock);
}

// Inventory manager waits for semaphore signal to restock
void* inventoryManagerThread(void* arg) {
    while (1) {
        sem_wait(&stockThreshold); // Wait for low-stock signal

        pthread_mutex_lock(&inventoryLock);
        for (int i = 0; i < numItems; i++) {
            if (inventory[i].quantity < 3) {
                int id = inventory[i].id;
                pthread_mutex_unlock(&inventoryLock);
                restockItem(id);
                logRestock(id, 10);
                break;
            }
        }
        pthread_mutex_unlock(&inventoryLock);
    }

    return NULL;
}

// Initialize threads
void initializeThreads() {
    sem_init(&stockThreshold, 0, 0);

    // Create inventory manager thread
    pthread_create(&managerThread, NULL, inventoryManagerThread, NULL);

    // Create customer threads
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&customerThreads[i], NULL, customerThread, id);
    }
}

// Clean up resources
void cleanupResources() {
    // Wait for customer threads
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customerThreads[i], NULL);
    }

    // Cancel inventory manager thread
    pthread_cancel(managerThread);
    pthread_join(managerThread, NULL);

    // Destroy semaphore
    sem_destroy(&stockThreshold);
}
