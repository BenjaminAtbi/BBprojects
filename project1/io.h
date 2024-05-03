#ifndef BB_IO

#define LED_PREFIX "/sys/class/leds/beaglebone:green:usr"
#define GPIO_PREFIX "/sys/class/gpio/gpio"
#define GPIO_EXPORT "/sys/class/gpio/export"

int exportGPIO(char* pinloc, int number);
int writeFile(char* filename, char* value);
int readLine(char* filename, char* buff, unsigned int maxLength);
#endif