/*
 * WSU Robotic's BB8 Team - Joystick to Bluetooth
 * 
 * Designed to take analog input from the connected joystick controller and write them
 * to the SoftwareSerial stream to be transmitted over bluetooth via the HC-05
 * tranceiver
 * 
 * Written by Jensen Reitz of WSU Robotics
 * 
 * - Utilizes the HC-05 Bluetooth Module to transmit the signal and thr HC-06 Bluetooth Module
 *    to recieve the signal
 */

#include <SoftwareSerial.h>

#define DEBUG

//Constant Variables -----------------------------------------------------------------
const int joystick_xPin = 3, joystick_yPin = 4;

// Global Variables ------------------------------------------------------------------
int joystick_x = 0, joystick_y = 0, past_joystick_x = 0, past_joystick_y = 0; //Values to be read in from the joystick (x is neutral at 504, y is neutral at 490)
String bt_str = "\0";

// Objects ---------------------------------------------------------------------------
SoftwareSerial bt_serial(2, 3); // RX | TX


// Start Program *********************************************************************


void setup() 
{
    // start the serial communication with the host computer
    Serial.begin(9600);
    Serial.println("Arduino with HC-05/HC-06 is ready");
 
    // start communication with the HC-06 using 9600
    bt_serial.begin(9600);
    Serial.println("bt_serial started at 9600");
}
 
void loop()
{
    //Read in values from the joystick
    joystick_x = analogRead(joystick_xPin);
    joystick_y = analogRead(joystick_yPin);

    if(signalFilter(joystick_x ,past_joystick_x) || signalFilter(joystick_y, past_joystick_y))
    {
      //Clear the string to rewrite it
      bt_str = "\0";
      
      //Parse values into string
      bt_str.concat(joystick_x);
      bt_str.concat(',');
      bt_str.concat(joystick_y);
      bt_str.concat('x');
      bt_str.concat('\n');
  
      past_joystick_x = joystick_x;
      past_joystick_y = joystick_y;
    }

      //Upload the string into bt_serial
      bt_serial.print(bt_str);
    
    #ifdef DEBUG
      Serial.print(bt_str);
    #endif
}


// Public Functions ********************************************************************


bool signalFilter(int newValue, int oldValue)
{
  if(abs(newValue-oldValue)>10)
    return true;
  else
    return false;
}
