#include <stdio.h>
#include "inventory.h"
#include "order_manager.h"
#include "logger.h"

int main() {
    initializeInventory();
    initializeThreads();
    cleanupResources();
    return 0;
}
