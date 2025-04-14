#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "inventory.h"
#include "order_manager.h"
#include "logger.h"

int main() {
    // Seed random number generator for order simulation
    srand(time(NULL));

    // Step 1: Initialize the inventory with predefined items
    initializeInventory();

    // Step 2: Start customer and manager threads
    initializeThreads();

    // Step 3: Wait for all threads to complete
    cleanupResources();

    printf("\n[Main] All threads completed. System shutting down.\n");
    return 0;
}
