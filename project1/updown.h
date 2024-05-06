#ifndef UPDOWN 


enum Joystick getJoystickVal();
void configJoysticks();

void setLights(char* setting);
void resetLights();
void flashLights(int miliseconds, float hz);
int randRange(int min, int max);
void sleepMiliseconds(int miliseconds);
void sleepRandMiliseconds(int min, int max);
double getTime();
#endif