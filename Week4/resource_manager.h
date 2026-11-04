#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "deadlock.h"

int request_resources(SystemState *state,
                      int process_id,
                      const int request[]);

void release_resources(SystemState *state,
                       int process_id,
                       const int release[]);

#endif

