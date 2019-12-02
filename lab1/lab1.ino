/**
 *  Student Names/Numbers:
 *  Zarif Shahriar, 8177206
 *  Pasoon Azimi, 8215497
 *  Rami Taleb, 8215184
 *  
 *  Group Number: 13
 *  Course Code: SEG 4145
 *  Lab Number: 1
 *  File name: lab1
 *  General Description: THe contents of this file contain the 
 *  required code to allow the StingRay robot to execute Paths 1 & 2
 */

#include <SoftwareSerial.h>
#include <stdio.h>

// Defining macros for wheel movement
#define LEFTFORWARD()      analogWrite(45, 250);
#define LEFTBACKWARD()     analogWrite(45, 10);
#define LEFTSTOP()       analogWrite(45, 0);
#define RIGHTFORWARD()     analogWrite(8, 128);
#define RIGHTBACKWARD()    analogWrite(8, 190);
#define RIGHTSTOP()      analogWrite(8, 0);

// Defining our global variables
// Initializing our LCD Screen
SoftwareSerial LCD = SoftwareSerial(0, 18);

int leftCircle = 3200;
int rightCircle = 3700;
int left90 = leftCircle/4;
int right90 = rightCircle/4;
int left135 = left90*1.5;
int right135 = right90*1.5;

void setup() {
    // Assigning our pins here

    // LED
    pinMode(13, OUTPUT);
    // Left Motor
    pinMode(45, OUTPUT);
    // Right Motor
    pinMode(8, OUTPUT);

    // Assigns our LCD Port
    LCD.begin(9600);

    // Clears our LCD
    clearLCD();

//    turnRight(20000);
//    stopMotor(3000);
    
    // Wait 2 seconds
    delay(2000);

    // Print First Student Number
    printPath("8215184");
    // Print Second Student Number
    printPath("8215497");

    // Print Path 1
    printPath("Path 1");
    // Execute the movements to complete Path 1
    executePath1();
    // Print Path 2
    printPath("Path 2");
    // Execute the movements to complete Path 2
    executePath2();
}

// No need for us to Loop, we only want to run the 2 paths once
void loop () {
  
}

/**
* Clears the characters on the LCD Screen
* @name clearLCD
* @return (void)
*/
void clearLCD() {
    LCD.write(0xFE);
    LCD.write(0x01);
    delay(10);
}

/**
* Prints the given message sent to the function
* @name printPath
* @param path the message that will be printed
* @return (void)
*/
void printPath(char* path) {
    clearLCD();
    LCD.write(0xFE);
    LCD.write(0 + 0*64 + 128);
    LCD.print(path);
    delay(3000);
}

/**
* Prints the given message sent to the function
* @name printMessage
* @param msg the message that will be printed
* @return (void)
*/
void printMessage(char* msg) {
    clearLCD();
    LCD.write(0xFE);
    LCD.write(0 + 0*64 + 128);
    LCD.print(msg);
    delay(10);
}

/**
* Stops the motor for a given duration
* @name stopMotor
* @param duration The # of millseconds that the robot will remain at rest
* @return (void)
*/
void stopMotor(int duration) {
    LEFTSTOP();
    RIGHTSTOP();
    delay(duration);
}

/**
* Moves the robot forward by moving both wheels forward for a given duration
* @name goForward
* @param duration The # of millseconds that the robot will move forward
* @return (void)
*/
void goForward(int duration) {
    LEFTFORWARD();
    RIGHTFORWARD();
    delay(duration);
}

/**
* Moves the robot left by moving the left wheel backwards and right wheel forwards for a given duration
* @name turnLeft
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnLeft(int duration) {
    LEFTBACKWARD();
    RIGHTFORWARD();
    delay(duration);
}

/**
* Moves the robot left by moving the left wheel forwards and right wheel backwards for a given duration
* @name turnRight
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnRight(int duration) {
    LEFTFORWARD();
    RIGHTBACKWARD();
    delay(duration);
}

/**
* This method calls the needed movement functions to complete path 1
* @name executePath1
* @return (void)
*/
void executePath1(){
    printMessage("Moving Forward");
    goForward(1000);
    stopMotor(500);

    printMessage("Turning Left 90");
    turnLeft(left90);
    stopMotor(500);

    printMessage("Moving Forward");
    goForward(1000);
    stopMotor(500);

    printMessage("Turning Right 90");
    turnRight(right90);
    stopMotor(500);

    printMessage("Moving Forward");
    goForward(1000);
    stopMotor(500);

    printMessage("Turning Right 90");
    turnRight(right90);
    stopMotor(500);

    printMessage("Moving Forward");
    goForward(1000);
    stopMotor(500);

    printMessage("Turning Left 90");
    turnLeft(left90);
    stopMotor(500);

    printMessage("Moving Forward");
    goForward(2000);

    printMessage("Finish Path 1");
    stopMotor(5000);
}

/**
* This method calls the needed movement functions to complete path 2
* @name executePath2
* @return (void)
*/
void executePath2(){
    printMessage("Moving Forward");
    goForward(1000);
    stopMotor(500);

    printMessage("Turning Right 90");
    turnRight(right90);
    stopMotor(500);

    printMessage("Moving Forward");
    goForward(1000);
    stopMotor(500);

    printMessage("Turning Left 135");
    turnLeft(left135); // 135 turn
    stopMotor(500);

    printMessage("Moving Forward");
    goForward(2500);
    stopMotor(500);

    printMessage("Turning Right 135");
    turnRight(right135); // 135 turn
    stopMotor(500);

    printMessage("Moving Forward");
    goForward(1000);
    stopMotor(500);

    printMessage("Turning Left 90");
    turnLeft(left90);
    stopMotor(500);
    
    printMessage("Moving Forward");
    goForward(1000);

    printMessage("Finish Path 2");
    stopMotor(5000);
}
