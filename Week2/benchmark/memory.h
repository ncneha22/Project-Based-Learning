#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 100

void memory_init(void);
void memory_store(int address, int value);
int memory_load(int address);

#endif

