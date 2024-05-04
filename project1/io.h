#ifndef BB_IO

#define LED_PREFIX  "/sys/class/leds/beaglebone:green:usr"
#define GPIO_PREFIX "/sys/class/gpio/gpio"
#define GPIO_EXPORT "/sys/class/gpio/export"

#define JOYSTICK_UP     GPIO_PREFIX "26"
#define JOYSTICK_DOWN   GPIO_PREFIX "46"
#define JOYSTICK_LEFT   GPIO_PREFIX "65"
#define JOYSTICK_RIGHT  GPIO_PREFIX "47"

#define VALUE       "/value"
#define DIRECTION   "/direction"
#define EDGE        "/edge"

#define LED_BRIGHTNESS LED_PREFIX "0/brightness"

int exportGPIO(char* pinloc, int number);
int writeFile(char* filename, char* value);
int readLine(char* filename, char* buff, unsigned int maxLength);


//lsb up, down, left, right
typedef unsigned char joystick_vals;

enum joystick_name {
    up = 1,
    down = 2,
    left = 4,
    right = 8,
};

#endif