#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <string.h> 
#include <errno.h>	
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "io.h"




int WaitForGpioEdge(char** gpio_files, int count)
{   
    int epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        printf("Error creating epoll fd: %s", strerror(errno));
        return -1;
    }

    struct epoll_event events[count];
    int gpio_fds[count];

    for(int i = 0; i < count; i++)
    {
        int gpiofd = open(gpio_files[i], O_RDONLY | O_NONBLOCK);
        gpio_fds[i] = gpiofd;
        if (gpiofd == -1) {
            printf("Failed to open GPIO value file (%s): %s", gpio_files[i], strerror(errno));
            return -1;
        }
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
        ev.data.fd = gpiofd;

        if(epoll_ctl(epollfd, EPOLL_CTL_ADD, gpiofd, &ev) == -1)
        {
            printf("Failed to add GPIO file descriptor to epoll instance: %s", strerror(errno));
            return -1;
        }
    }

    for (int i = 0; i < 2; i++) {
		int waitRet = epoll_wait(epollfd, events, 
				count,                // maximum # events
				-1);              // timeout in ms, -1 = wait indefinite; 0 = returne immediately

		if (waitRet == -1){
			fprintf(stderr, "ERROR: epoll_wait() failed (%d) = %s\n", waitRet, strerror(errno));
			for(int i = 0; i < count; i++)
            {
                close(gpio_fds[i]);
            }
			close(epollfd);
			return -1;
		}
	}
    for(int i = 0; i < count; i++)
    {
        close(gpio_fds[i]);
    }
    close(epollfd);
    return 0;
}

joystick_vals getJoystickVals() {
    char* joystick_filenames[4] = {
        JOYSTICK_UP VALUE, 
        JOYSTICK_DOWN VALUE,
        JOYSTICK_LEFT VALUE,
        JOYSTICK_RIGHT VALUE,
    };
    joystick_vals values = 0;

    for(int i = 0; i < 4; i++)
    {
        char buff[10];
        readLine(joystick_filenames[i], buff, 10);
        if(buff[0] != '0' && buff[0] != '1')
        {
            printf("Unexpected joystick value in %s\n", joystick_filenames[i]);
            return -1;
        }
        values = (values<<1) + (buff[0] - 48);
    }
    return values;
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
    

int main(int argc, char* args[])
{ 
    configJoysticks();
    char* joystick_filenames[4] = {
        JOYSTICK_DOWN VALUE, 
        JOYSTICK_UP VALUE,
        JOYSTICK_LEFT VALUE,
        JOYSTICK_RIGHT VALUE,
    };
    
    while(1)
    {
        printf("Get Ready!\n");

        //if pressing joystick
        //please let go
        //loop polling until they let go
        joystick_vals values;
        if(values = getJoystickVals() == -1) exit(1);
        if (values != 15)
        {
            printf("Please let go of the stick\n");
            while(values!=15)
            {
                if(WaitForGpioEdge(joystick_filenames,4) == -1) exit(1);
                if(values = getJoystickVals() == -1) exit(1);
            }
        }
        
        struct timespec delay = { }
        
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

