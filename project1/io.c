#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "io.h"


int exportGPIO(char* pinloc, int number)
{
    if(access(pinloc, F_OK) == 1)
    {
        
        FILE *pfile = fopen(GPIO_EXPORT, "w");
        if (pfile == NULL)
        {
            printf("ERROR: Unable to open export file %s. \n", pinloc);
            return -1;
        }

        fprintf(pfile, "%d", number);
        fclose(pfile);
    }
    return 0;
}

int writeFile(char* filename, char* value)
{
    FILE *pfile = fopen(filename, "w");
    if (pfile == NULL)
    {
        printf("ERROR: Unable to write to %s. \n", filename);
        return -1;
    }
    fprintf(pfile, "%s", value);
    fclose(pfile);
    return 0;
}

int readLine(char* filename, char* buff, unsigned int maxLength)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("ERROR: Unable to read from %s. \n", filename);
        exit(1);
    }
    int bytes_read = getline(&buff, &maxLength, file);
    fclose(file);
    return bytes_read;
}