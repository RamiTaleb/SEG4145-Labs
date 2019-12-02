/**
* Student Names/Numbers:
*  Zarif Shahriar, 8177206
*  Pasoon Azimi, 8215497
*  Rami Taleb, 8215184
*  Group Number: 13
*  Course Code: SEG 4145
*  Lab Number: 4
*  File name: lab4
* This file is responsible for establishing a connection to a WiFi network using
* the robot and then using a PC to exchange data with the robot and execute
* various commands in real-time
*
*/

#include <SoftwareSerial.h>
#include <stdio.h>
#include <Wire.h>
#include <Wirefree.h>
#include <WifiClient.h>

//Macros to simplify functions
#define LEFT_FORWARD()    analogWrite(45, 250);
#define LEFT_BACKWARD()   analogWrite(45, 50);
#define LEFT_STOP()       analogWrite(45, 0);
#define RIGHT_FORWARD()   analogWrite(8, 128);
#define RIGHT_BACKWARD()  analogWrite(8, 250);
#define RIGHT_STOP()      analogWrite(8, 0);
#define TEMPSENSOR        0x68

//Setup wireless profile for robot
WIFI_PROFILE wireless_prof = {"stingray","","192.168.1.28","255.255.255.0","192.168.1.1"};

String remote_server = "192.168.1.164";
String remote_port = "5878";

WifiClient client(remote_server, remote_port, PROTO_TCP);

// Global variables
int flashing, sonar;
int turning = 0;
long ping;
long distance;
byte temperatureData;
SoftwareSerial LCD = SoftwareSerial(0, 18); //Initialize the LCD screen
int reg = 0x01; //For ambient temperature reader

int command = 0; //Keep track of user command
int commandInput[3] = {-1, -1, -1};
int count; //Keep track of the current byte
int currentLineIsBlank;

/* 
 * setup()
 * void
 * void
 * Setup code
 */
 void setup() {
    //Activating pins for the sensors and actuators
    pinMode(13, OUTPUT); // LED
    pinMode(45, OUTPUT); // Left Motor
    pinMode(8, OUTPUT); // Right Motor
    pinMode(49, INPUT); // Left wheel sensor
    pinMode(48, INPUT); // Right wheel sensor

    //Setting up LCD screen and printing student numbers
    LCD.begin(9600);
    clearLCD();
    LCD.write(0xFE);
    LCD.write(5 + 0*64 + 128);
    delay(10);
    LCD.print("8185938");
    delay(10);
    LCD.write(0xFE);
    LCD.write(5 + 1*64 + 128);
    LCD.print("8273487");
    delay(10);

    //Flash LED for a period of 5 seconds
    flashLight(5);
    clearLCD();

    //I2C bus for getting ambient temperature
    Wire.begin();
    //Begin wireless connnection
    Wireless.begin(&wireless_prof);

    // Try and connect to client
    if (client.connect()) {
        // prints to serial if connected
        Serial.println("Connection successful.");

        // Send message over UDP socket to peer device
        client.println("Robot now connected.");
    } else {
        // prints to serial if not connected
        Serial.println("Connection failed.");
    }
}

// the loop function runs over and over again forever
void loop() {
    // if there are incoming bytes available from the peer
    // device, read them and print them:
    while (client.available()) {

        int a;
  char c;

        while ((c = client.read()) == -1); // While nothing returned

    // Assign int to char
    //a = c; // may need to add -->  + '0'

    // If new line is reached and current line is blank
    if (c == '\n' && currentLineIsBlank == 1) {

      // Reset count
      count = 0;

      // Create empty array of size 2
      int action[2] = {};

      // Store command in first spot
      action[0] = command;
                        Serial.println(action[0]);

      // Get input and store in second spot
      int i, k = 0;
      for (i = 0; i < 3; i++) {
        if (commandInput[i] != -1) {
          k = 10 * k + commandInput[i];
        }
      }
      action[1] = k;
                        Serial.println(action[1]);

      // Process user action
      processUserAction(action);

      // Reset command array
      commandInput[0] = -1;
                        commandInput[1] = -1;
                        commandInput[2] = -1;

      // Send response to server
      client.println("Robot has responded, waiting for next instruction.");

      // Break current iteration of while loop
      continue;
    }

    // If new line
    if (c == '\n') {
      currentLineIsBlank = 1;
      count = 0;
    } else if (c != '\r') { // If byte received
      currentLineIsBlank = 0;

      // Get the command
      if (count == 0) {

        if (c == '07') {
          Serial.println("Robot quitting.");
          break;
        }

        // Store as int
        command = c - '0';
      } else {
                // Add to array
                commandInput[count-1] = c - '0';
            }

      // Increment count
      count++;

    }
    }

    // Delay for 1 millisecond
    delay(1);
}

/**
* Clears all characters from the LCD screen.
* @name clearLCD
* @return (void)
*/
void clearLCD() {
    LCD.write(0xFE);  // Put the LCD screen in command mode.
    LCD.write(0x01);  // Clear the LCD screen.
    delay(10);
}

/**
* Prints a message to the LCD screen.
* @name printMessage
* @param word1 word on first line, word2 word of second line
* @return (void)
*/
void printMessage(char* word1, char* word2) {
    // Clear the LCD
    clearLCD();

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Get word1 length
    int word1Len = strlen(word1);

    // Set cursor to first row, first column
    LCD.write(((16-word1Len)/2) + 0*64 + 128);

    // Print the message
    LCD.print(word1);

    // If there is another word
    if (word2 != 0) {
        // Put the LCD screen in command mode.
        LCD.write(0xFE);

        // Get word1 length
        int word2Len = strlen(word2);

        // Set cursor to first row, first column
        LCD.write(((16-word2Len)/2) + 1*64 + 128);

        // Print the message
        LCD.print(word2);
    }

    // Delay for 10 milliseconds
    delay(10);
}

/**
* Prints temperature to LCD Screen
* @name printTemperature
* @param word1 word on first line, word2 word of second line
* @return (void)
*/
void printTemperature(char* word1, byte word2) {
    // Clear the LCD
    clearLCD();

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Get word1 length
    int word1Len = strlen(word1);

    // Set cursor to first row, first column
    LCD.write(((16-word1Len)/2) + 0*64 + 128);

    // Print the message
    LCD.print(word1);

    // If there is another word
    if (word2 != 0) {
        // Put the LCD screen in command mode.
        LCD.write(0xFE);

        // Set cursor to first row, first column
        LCD.write(0 + 1*64 + 128);

        // Print the message
        LCD.print(word2);
    }

    // Delay for 10 milliseconds
    delay(10);
}

/**
* Prints distance of closest object to LCD Screen
* @name printDistance
* @param word1 word on first line, word2 word of second line
* @return (void)
*/
void printDistance(char* word1, long word2) {
    // Clear the LCD
    clearLCD();

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Get word1 length
    int word1Len = strlen(word1);

    // Set cursor to first row, first column
    LCD.write(0 + 0*64 + 128);

    // Print the message
    LCD.print(word1);

    // If there is another word
    if (word2 != 0) {
        // Put the LCD screen in command mode.
        LCD.write(0xFE);

        // Set cursor to first row, first column
        LCD.write(0 + 1*64 + 128);

        // Print the message
        LCD.print(word2);
    }

    // Delay for 10 milliseconds
    delay(10);
}

/**
* Flashes the light every second for num amount of time.
* @name flashLight
* @param num number of seconds that the light will be flashing
* @return (void)
*/
void flashLight(int num) {
    for (flashing = 0; flashing < num; flashing++) {
        // Wait for 1s
        delay(1000);

        // turn the LED on (HIGH is the voltage level)
        digitalWrite(13, HIGH);

        // Wait for 0.01s
        delay(10);

        // turn the LED on (HIGH is the voltage level)
        digitalWrite(13, LOW);
    }
}

/**
* Adjust the speed of the wheels
* @name adjustSpeed
* @param motor number of motor
* @return (void)
*/
void adjustSpeed(int motor) {

    if (motor == 45) {
      RIGHT_FORWARD();
      analogWrite(motor, 150);
    } else {
      LEFT_FORWARD();
      analogWrite(motor, 100);
    }

}

/**
* Performs an action for a predefined number of "ticks"
* @name actionLength
* @param ticks number of wheel ticks that must be processed
* @return (void)
*/
void actionLength(int ticks) {
    int timeLeft = 0;
    int timeRight = 0;
    int oldPulseLeft = NULL;
    int newPulseLeft = NULL;
    int oldPulseRight = NULL;
    int newPulseRight = NULL;
    int valid = 0;

    while (valid == 0) {

        // Read left motor  sensor
        oldPulseLeft = newPulseLeft;
        newPulseLeft = digitalRead(48);

        // Read right motor sensor
        oldPulseRight = newPulseRight;
        newPulseRight = digitalRead(49);

        // If new tick, increment left time
        if (newPulseLeft != oldPulseLeft) {
            timeLeft++;
        }

        // If new tick, increment right time
        if (newPulseRight != oldPulseRight) {
            timeRight++;
        }

        // If the robot is not currently turning
        if (turning != 1) {

          // Check if sensors are equal or not
          if (timeLeft > timeRight) {
//              adjustSpeed(45);
          } else if (timeRight > timeLeft) {
//              adjustSpeed(8);
          }
        }
        
        // Set valid flag to true if both are done    
        if (timeLeft >= ticks && timeRight >= ticks) {
          valid = 1;
        }
    }
}

/**
* Retrieves user action and processes it accordingly
* @name processUserAction
* @params
* @return (void)
*/
void processUserAction(int input[]) {
    // Store selection
    int selection = input[0];
    
    //Serial.println(input[0]);

    // Switch case
    switch(selection) {
        case 1:
            moveForward(calculateDistance(input[1]));
            stopMotor(10);
            break;
        case 2:
            moveBackward(calculateDistance(input[1]));
            stopMotor(10);
            break;
        case 3:
            turnRight(calculateDegrees(input[1]));
            stopMotor(10);
            break;
        case 4:
            turnLeft(calculateDegrees(input[1]));
            stopMotor(10);
            break;
        case 5:
            readSonar();
            printDistance("Distance", distance);
            break;
        case 6:
            readTemp(input[1]);
            printTemperature("Temperature", temperatureData);
            break;
    }
}

/**
* Takes cm distance and converts to ticks
* @name calculateDistance
* @params distance distance in cm
* @return int number of ticks
*/
int calculateDistance(int distance) {
    return (distance * 4);
}

/**
* Takes degrees and converts to ticks
* @name calculateDegrees
* @params degrees degrees in
* @return int number of ticks
*/
int calculateDegrees(long degrees) {
    return (degrees * 0.5);
}

/**
* Read the sonar results and calculates the resulting distance
* @name readSonar
* @return (void)
*/
void readSonar(){

    // Output Sonar sensor
    pinMode(22, OUTPUT);

    // Write low value to pin
    digitalWrite(22, LOW);

    //Wait for 2 microsecs
    delay(0.002);

    // Write high value to pin
    digitalWrite(22, HIGH);

    //Wait for 5 microsecs
    delay(0.005);

    //Write low value to pin
    digitalWrite(22, LOW);

    // Input Sonar sensor
    pinMode(22, INPUT);

    // Read duration of the pulse
    ping = pulseIn(22, HIGH);

    // Calculate distance
    distance = ping / (29*2);

    // Delay 10 milliseconds
    delay(10);
}

/**
* Reads the ambient temperature
* @name readTemp
* @return (void)
*/
void readTemp(int type) {
    Wire.beginTransmission(TEMPSENSOR);
    Wire.write(reg); // Indicate temperature value to read
    Wire.endTransmission();
    Wire.requestFrom(TEMPSENSOR, 1); // Request data
    while(Wire.available() < 1); // Wait for data
    temperatureData = Wire.read(); // Store temperature value
  // Change if fahrenheit
  if (type == 2) {
    temperatureData = temperatureData * 1.8 + 32;
  }
    delay(50); // Delay 50 ms if values are read in a loop
}

/**
* Stops the robot. A delay is provided to determine how long
* the robot will remain in this state.
* @name stopMotor
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void stopMotor(int duration) {
    LEFT_STOP();
    RIGHT_STOP();
    turning = 0;
    printMessage("Stopped", 0);
    delay(duration);
}

/**
* Moves the robot forward. A delay is provided to determine how long
* the robot will remain in this state.
* @name moveForward
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void moveForward(int duration) {
    LEFT_FORWARD();
    RIGHT_FORWARD();
    turning = 0;
    printMessage("Moving", "Forward");
    actionLength(duration);
}

/**
* Moves the robot backward. A delay is provided to determine how long
* the robot will remain in this state.
* @name moveBackward
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void moveBackward(int duration) {
    LEFT_BACKWARD();
    RIGHT_BACKWARD();
    turning = 1; // Flag so wheels don't get adjusted
    printMessage("Moving", "Backward");
    actionLength(duration);
}

/**
* Turns the robot left. A delay is provided to determine how long
* the robot will remain in this state.
* @name turnLeft
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnLeft(int duration) {
    LEFT_BACKWARD();
    RIGHT_FORWARD();
    turning = 1;
    printMessage("Rotating", "Left");
    actionLength(duration);
}

/**
* Turns the robot right. A delay is provided to determine how long
* the robot will remain in this state.
* @name turnRight
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnRight(int duration) {
    LEFT_FORWARD();
    RIGHT_BACKWARD();
    turning = 1;
    printMessage("Rotating", "Right");
    actionLength(duration);
}
