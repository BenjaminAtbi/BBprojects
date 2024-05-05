#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>	
#include <string.h> 
#include <sys/epoll.h>
#include <fcntl.h>
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