#pragma once
#include <stdbool.h>

struct ValueSem;
typedef struct ValueSem ValueSem;

ValueSem* Create(void);
void Destroy(ValueSem* var);
void Insert(ValueSem* var, void* value);
void* Export(ValueSem* var);
