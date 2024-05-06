#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <string.h> 
#include <errno.h>	
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include "io.h"
#include "updown.h"


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

void setLights(char* setting)
{
    writeFile(LED_0 BRIGHTNESS, setting);
    writeFile(LED_1 BRIGHTNESS, setting);
    writeFile(LED_2 BRIGHTNESS, setting);
    writeFile(LED_3 BRIGHTNESS, setting);
}

void resetLights()
{
    writeFile(LED_0 TRIGGER, "none");
    writeFile(LED_1 TRIGGER, "none");
    writeFile(LED_2 TRIGGER, "none");
    writeFile(LED_3 TRIGGER, "none");
    setLights("0");
}

void flashLights(int miliseconds, float hz)
{
    //each cycle has two segments (on and off)
    int ms_per_segment = 500 / hz;
    int num_segments = miliseconds / ms_per_segment;
    int ms_remainder = miliseconds % ms_per_segment;
    int i = 0;
    for(; i < num_segments; i++) {
        if(i % 2 == 0) setLights("1");
        else setLights("0");
        sleepMiliseconds(ms_per_segment);
    }
    if(ms_remainder > 0) {
        if(i % 2 == 0) setLights("1");
        else setLights("0");
        sleepMiliseconds(ms_remainder);
    }
}

int randRange(int min, int max)
{
    int offset = max - min + 1;
    return (rand() % offset) + min;
}

void sleepMiliseconds(int miliseconds)
{
    int seconds = miliseconds / 1000;
    int nanoseconds = (miliseconds % 1000) * 1000000;
    struct timespec delay = {seconds, nanoseconds};
    nanosleep(&delay, (struct timespec*) NULL);
}

void sleepRandMiliseconds(int min, int max)
{
    int rand_miliseconds = randRange(500, 3000);
    sleepMiliseconds(rand_miliseconds);
}

double getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
    return seconds; 
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
    
    double fastest_time = 999.9;

    int iteration = 0;
    while(1)
    {
        iteration++;
        printf("Get Ready!\n");

        //wait until player is not using the stick
        enum Joystick value;
        if((value = getJoystickVal()) == -1) exit(1);
        if (value != NONE)
        {
            printf("Please let go of the stick\n");
            while(value != NONE)
            {
                if((WaitForGpioEdge(joystick_filenames,4,-1)) == -1) exit(1);
                if((value = getJoystickVal()) == -1) exit(1);
            }
            printf("Get Ready!\n");
        }

        //wait .5 to 3 seconds
        sleepRandMiliseconds(500,3000);
        
        //restart if they press up or down too fast, exit if wrong direction
        if((value = getJoystickVal()) == -1) exit(1);
        if(value == UP || value == DOWN) 
        {
            printf("Too Soon!\n");
            // printf("1, iteration %d\n",iteration);
        } else if (value == LEFT || value == RIGHT)
        {
            printf("Illegal direction, you suck!\n");
            // printf("2, iteration %d\n", iteration);
            exit(0);
        } else {
            // printf("3, iteration %d\n", iteration);
            //select direction
            enum Joystick direction = randRange(0,1);
            if(direction == UP)
            {
                printf("UP!\n"); 
                writeFile(LED_0 BRIGHTNESS, "1"); 
            } else if(direction == DOWN){
                printf("DOWN!\n"); 
                writeFile(LED_3 BRIGHTNESS, "1"); 
            }

            //time directional push
            double time_start = getTime();
            if((WaitForGpioEdge(joystick_filenames,4,5000)) == -1) exit(1);
            double time_end = getTime();
            double duration = (time_end - time_start);

            //end program if they took too long
            if(duration >= 5.0)
            {
                printf("You're too slow!\n");
                exit(0);
            }

            //process directional press
            if((value = getJoystickVal()) == -1) exit(1);
            if(value == direction)
            {
                printf("Correct direction! \n");
                if(duration < fastest_time)
                {
                    printf("Fastest time yet!\n");
                    fastest_time = duration;
                }
                printf("last time -- %f\n",duration);
                printf("best time -- %f\n",fastest_time);
                flashLights(500,4);
            } else if(value == UP || value == DOWN)
            {
                printf("Wrong Direction\n");
                flashLights(1000,10);
            } else {
                printf("Illegal direction, you suck!\n");
                exit(0);
            }
        }
        //restart game when joystick let go
        if((value = getJoystickVal()) == -1) exit(1);
        while(value != NONE)
        {
            if((WaitForGpioEdge(joystick_filenames,4,-1)) == -1) exit(1);
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

