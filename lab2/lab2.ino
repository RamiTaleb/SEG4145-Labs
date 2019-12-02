/**
 *  Student Names/Numbers:
 *  Zarif Shahriar, 8177206
 *  Pasoon Azimi, 8215497
 *  Rami Taleb, 8215184
 *  
 *  Group Number: 13
 *  Course Code: SEG 4145
 *  Lab Number: 2
 *  File name: lab2
 *  General Description: THe contents of this file contain the 
 *  required code to allow the StingRay robot to execute Paths 1 & 2
 */

#include <SoftwareSerial.h>
#include <stdio.h>
#include <Servo.h>

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

Servo right;
Servo left;

int leftCircle = 3200;
int rightCircle = 3700;
int left90 = 950;
int right90 = 1100;
int left135 = 1350;
int right135 = right90*1.5;
int left45 = left90/2;
int right45 = right90/2;

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
    
    // Wait 2 seconds
    delay(2000);

    // Print First Student Number
    printPath("8215184");
    // Print Second Student Number
    printPath("8215497");

    printPath("Path");
    // Execute the movements to complete Path 
    executePath();
 
}

// No need for us to Loop, we only want to run the 2 paths once
void loop () {
  
}

void start(){
  right.attach(8);
  left.attach(45);
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
    printMessage("Moving Forward");
    right.write(0);
    left.write(180);
    delay(duration);
}

/**
* Moves the robot left by moving the left wheel backwards and right wheel forwards for a given duration
* @name turnLeft
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnLeft(int duration) {
    printMessage("Turning Left");
    right.write(15);
    left.write(70);
    delay(duration);
}

/**
* Moves the robot left by moving the left wheel forwards and right wheel backwards for a given duration
* @name turnRight
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnRight(int duration) {
    printMessage("Turning Right");
    right.write(83);
    left.write(90);
    delay(duration);
}

void moveNumberOfBlocks(int num){
  printMessage("Moving Forward");
  goForward(num*1000);
}

/**
* This method calls the needed movement functions to complete the path 
* @name executePath
* @return (void)
*/
void executePath(){
    start();
    moveNumberOfBlocks(2);
    turnRight(right90);
    
    moveNumberOfBlocks(2);
    turnRight(right90);

    moveNumberOfBlocks(3);
    turnRight(right90);

    moveNumberOfBlocks(3);
    turnRight(right90);

    moveNumberOfBlocks(2);
    turnLeft(left45);

    moveNumberOfBlocks(1);
    turnLeft(left135);

    moveNumberOfBlocks(4);
    turnLeft(left90);

    moveNumberOfBlocks(2);
    turnLeft(left90);
    
    moveNumberOfBlocks(2);
}
