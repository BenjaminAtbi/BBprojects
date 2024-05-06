#ifndef BB_IO

#define LED_PREFIX  "/sys/class/leds/beaglebone:green:usr"
#define GPIO_PREFIX "/sys/class/gpio/gpio"
#define GPIO_EXPORT "/sys/class/gpio/export"

#define LED_0 LED_PREFIX "0/"
#define LED_1 LED_PREFIX "1/"
#define LED_2 LED_PREFIX "2/"
#define LED_3 LED_PREFIX "3/"

#define JOYSTICK_UP     GPIO_PREFIX "26/"
#define JOYSTICK_DOWN   GPIO_PREFIX "46/"
#define JOYSTICK_LEFT   GPIO_PREFIX "65/"
#define JOYSTICK_RIGHT  GPIO_PREFIX "47/"

#define VALUE       "value"
#define DIRECTION   "direction"
#define EDGE        "edge"
#define TRIGGER     "trigger"
#define BRIGHTNESS  "brightness"

int exportGPIO(char* pinloc, int number);
int writeFile(char* filename, char* value);
int readLine(char* filename, char* buff, unsigned int maxLength);
int WaitForGpioEdge(char** gpio_files, int count, int timeout);

enum Joystick {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
};

#endif