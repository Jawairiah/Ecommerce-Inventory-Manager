#ifndef LOGGER_H
#define LOGGER_H

void logOrder(int customerId, int itemId, int quantity, int success);
void logRestock(int itemId, int addedQuantity);

#endif
