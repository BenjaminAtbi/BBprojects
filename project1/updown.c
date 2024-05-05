#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <string.h> 
#include <errno.h>	
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "io.h"


enum Joystick getJoystickVal() {
    char* joystick_filenames[4] = {
        JOYSTICK_UP VALUE, 
        JOYSTICK_DOWN VALUE,
        JOYSTICK_LEFT VALUE,
        JOYSTICK_RIGHT VALUE,
    };

    for(int i = 0; i < 4; i++)
    {
        char buff[10];
        readLine(joystick_filenames[i], buff, 10);
        if(buff[0] != '0' && buff[0] != '1')
        {
            printf("Unexpected joystick value in %s\n", joystick_filenames[i]);
            return -1;
        }
        if (buff[0] == '0') return i;
    }
    return NONE;
}

void configJoysticks(){
    writeFile(JOYSTICK_UP DIRECTION, "in");
    writeFile(JOYSTICK_UP EDGE, "both");
    writeFile(JOYSTICK_DOWN DIRECTION, "in");
    writeFile(JOYSTICK_DOWN EDGE, "both");
    writeFile(JOYSTICK_LEFT DIRECTION, "in");
    writeFile(JOYSTICK_LEFT EDGE, "both");    
    writeFile(JOYSTICK_RIGHT DIRECTION, "in");
    writeFile(JOYSTICK_RIGHT EDGE, "both");
}

void resetLights()
{
    writeFile(LED_0 TRIGGER, "none");
    writeFile(LED_1 TRIGGER, "none");
    writeFile(LED_2 TRIGGER, "none");
    writeFile(LED_3 TRIGGER, "none");
    writeFile(LED_0 BRIGHTNESS, "0");
    writeFile(LED_1 BRIGHTNESS, "0");
    writeFile(LED_2 BRIGHTNESS, "0");
    writeFile(LED_3 BRIGHTNESS, "0");
}



int randRange(int min, int max)
{
    int offset = max - min + 1;
    return (rand() % offset) + min;
}

void sleepMiliseconds(int min, int max)
{
    int rand_miliseconds = randRange(500, 3000);
    int seconds = rand_miliseconds / 1000;
    int nanoseconds = (rand_miliseconds % 1000) * 1000000;
    struct timespec delay = {seconds, nanoseconds};
    nanosleep(&delay, (struct timespec*) NULL);
}


int main(int argc, char* args[])
{ 
    configJoysticks();
    resetLights();

    char* joystick_filenames[4] = {
        JOYSTICK_DOWN VALUE, 
        JOYSTICK_UP VALUE,
        JOYSTICK_LEFT VALUE,
        JOYSTICK_RIGHT VALUE,
    };
    
    while(1)
    {
        printf("Get Ready!\n");

        //wait until player is not using the stick
        enum Joystick value;
        if((value = getJoystickVal()) == -1) exit(1);
        if (value != NONE)
        {
            printf("Please let go of the stick\n");
            while(value != NONE)
            {
                if((WaitForGpioEdge(joystick_filenames,4)) == -1) exit(1);
                if((value = getJoystickVal()) == -1) exit(1);
            }
            printf("Get Ready!\n");
        }

        //wait .5 to 3 seconds
        sleepMiliseconds(500,3000);
        
        if((value = getJoystickVal()) == -1) exit(1);
        if(value == UP || value == DOWN) 
        {
            printf("Too Soon!\n");
        } else 
        {
            enum Joystick direction = randRange(0,1);
            switch(direction)
            {
                case UP:    
                    printf("UP!\n"); 
                    writeFile(LED_0 BRIGHTNESS, "1"); 
                    break;
                case DOWN:  
                    printf("DOWN!\n"); 
                    writeFile(LED_3 BRIGHTNESS, "1"); 
                    break;
                default: break;
            }

            clock_t time_start = clock();
            if((WaitForGpioEdge(joystick_filenames,4)) == -1) exit(1);
            if((value = getJoystickVal()) == -1) exit(1);
            
        }
    }
    
    // FILE *pLedTriggerFile = fopen(LED_BRIGHTNESS, "w");
    
    // if(pLedTriggerFile == NULL) {
    //     printf("error opening %s.", LED_BRIGHTNESS);
    //     exit(1);
    // }

    // int charWritten = fprintf(pLedTriggerFile, "1");
    // if(charWritten <= 0) {
    //     printf("error writing data");
    //     exit(1);
    // }
    // fclose(pLedTriggerFile);
    return 0;
}

