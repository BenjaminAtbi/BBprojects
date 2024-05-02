#include <stdlib.h>
#include <stdio.h>
#include "lights.h"

int main(int argc, char* args[])
{
    FILE *pLedTriggerFile = fopen(LED0 "/brightness", "w");
    
    if(pLedTriggerFile == NULL) {
        printf("ERROR OPENING %s.", LED0 "/brightness");
        exit(1);
    }

    int charWritten = fprintf(pLedTriggerFile, "1");
    if(charWritten <= 0) {
        printf("ERROR WRITING DATA");
        exit(1);
    }
    fclose(pLedTriggerFile);
    return 0;
}