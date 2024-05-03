#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <string.h> 
#include <errno.h>	
#include <unistd.h>
#include <fcntl.h>
#include "io.h"

#define JOYSTICK_UP_VALUE GPIO_PREFIX "26/value"
#define JOYSTICK_UP_DIRECTION GPIO_PREFIX "26/direction"
#define JOYSTICK_UP_EDGE GPIO_PREFIX "26/edge"

#define LED_BRIGHTNESS LED_PREFIX "0/brightness"

int WaitForGpioEdge(char* gpio_file)
{   
    int gpiofd = open(gpio_file, O_RDONLY | O_NONBLOCK);
    if (gpiofd == -1) {
        printf("Failed to open GPIO value file (%s): %s", gpio_file, strerror(errno));
        return -1;
    }

    int epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        printf("Error creating epoll fd: %s", strerror(errno));
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

    for (int i = 0; i < 2; i++) {
		int waitRet = epoll_wait(epollfd, &ev, 
				1,                // maximum # events
				-1);              // timeout in ms, -1 = wait indefinite; 0 = returne immediately

		if (waitRet == -1){
			fprintf(stderr, "ERROR: epoll_wait() failed (%d) = %s\n", waitRet, strerror(errno));
			close(gpiofd);
			close(epollfd);
			return -1;
		}
        printf("waitret: %d\n", waitRet);
	}
    close(gpiofd);
    close(epollfd);
    return 0;
}

int closeEpoll(int epollfd)
{
    if (close(epollfd)) {
		printf("Failed to close epoll file descriptor: %s", strerror(errno));
		return 1;
	}
    return 0;
}

int main(int argc, char* args[])
{ 
    writeFile(JOYSTICK_UP_DIRECTION, "in");
    writeFile(JOYSTICK_UP_EDGE, "both");
    
    while(1)
    {
        //get ready

        //if pressing joystick
        //please let go
        //loop polling until they let go
        printf("Get Ready!\n");
        
        int ret = WaitForGpioEdge(JOYSTICK_UP_VALUE);
        if (ret == -1) {
			exit(EXIT_FAILURE);
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

