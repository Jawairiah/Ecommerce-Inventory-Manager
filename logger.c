#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "logger.h"
#include "inventory.h"

// Mutex for log file access
pthread_mutex_t logMutex = PTHREAD_MUTEX_INITIALIZER;

// Helper: Get current timestamp string
void getTimestamp(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

// Log order attempts (success or failure)
void logOrder(int customerId, int itemId, int quantity, int success) {
    FILE* fp;
    char timestamp[30];

    pthread_mutex_lock(&logMutex);

    fp = fopen("log.txt", "a");
    if (fp == NULL) {
        perror("Error opening log file");
        pthread_mutex_unlock(&logMutex);
        return;
    }

    getTimestamp(timestamp, sizeof(timestamp));
    fprintf(fp, "[%s] [ORDER] Customer %d tried to buy %d of Item ID %d — %s\n",
            timestamp, customerId, quantity, itemId,
            success ? "SUCCESS" : "FAILED");

    fclose(fp);
    pthread_mutex_unlock(&logMutex);
}

// Log restocking events
void logRestock(int itemId, int addedQuantity) {
    FILE* fp;
    char timestamp[30];

    pthread_mutex_lock(&logMutex);

    fp = fopen("log.txt", "a");
    if (fp == NULL) {
        perror("Error opening log file");
        pthread_mutex_unlock(&logMutex);
        return;
    }

    getTimestamp(timestamp, sizeof(timestamp));
    fprintf(fp, "[%s] [RESTOCK] Item ID %d restocked by %d units.\n",
            timestamp, itemId, addedQuantity);

    fclose(fp);
    pthread_mutex_unlock(&logMutex);
}
