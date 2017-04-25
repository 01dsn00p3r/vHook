#include "main.h"



int __attribute__((constructor)) Init() {
    /* do shiz her. */
    
    InitializeInterfaces();
    
    InitializeVMTs();
    
    InitializeHooks();
    
    NetVarManager::DumpNetvars();
    
    Offsets::getOffsets();
    
    UpdateResolver();
    
    return 0;
}

void __attribute__((constructor)) DeInit() {
}
