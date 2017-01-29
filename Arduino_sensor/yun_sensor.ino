/*********************************************************************************
SMART PARKING LOT SYSTEM
*********************************************************************************/
#include <ArduinoJson.h>

//Reference Value to Get The Status of Parking
#define g_parked 10
#define g_fault 4500

//Pin Setting for the 3 Ultrasonic Sensors
#define g_trigPin 13
#define g_echoPin 12
#define g_trigPin2 4
#define g_echoPin2 5
#define g_trigPin3 8
#define g_echoPin3 9

int tmp_state,tmp_state2,tmp_state3,state,state2,state3;

/**************************************************************************************
Function Name     : setup
Description       : Initialize the Sensor Trigger Pin as Output and 
                    echo pin as Input and begin the serial communication
                    with 9600 BAUD
Parameters        : void
Return            : void
**************************************************************************************/
//Initialize the Pins and BAUD rate 
void setup(void) {
        //Serial1.begin (9600);
        //Serial.begin(9600);
        pinMode(g_trigPin, OUTPUT);
        pinMode(g_echoPin, INPUT);
        pinMode(g_trigPin2, OUTPUT);
        pinMode(g_echoPin2, INPUT);
        pinMode(g_trigPin3, OUTPUT);
        pinMode(g_echoPin3, INPUT);

        SERIAL_PORT_USBVIRTUAL.begin(115200);  // open serial connection via USB-Serial
          

        // setting up Mtdot LoRa and connecting to Gateway
        SERIAL_PORT_HARDWARE.begin(115200);
        delay(2000);
        SERIAL_PORT_HARDWARE.println("AT+NI=1,MultiTech");
        delay(2000);
        SERIAL_PORT_HARDWARE.println("AT+NK=1,MultiTech");
        delay(2000);
        SERIAL_PORT_HARDWARE.println("AT+FSB=1");
        delay(2000);
        SERIAL_PORT_HARDWARE.println("AT+JOIN");
        delay(2000);

         // initialize states
         state = 2;
         state2 = 2;
         state3 = 2;


}

/**************************************************************************************
Function Name     : loop
Description       : Generate a Trigger signal and wait for the echo on 
                    HC-SR04 Ultrasonic Sensor and calculate the distance.
Parameters        : void
Return            : void
**************************************************************************************/
void loop(void) {
  //Variables to calculate the distance using the duration taken for 1 cycle of trigger and echo      
  long l_duration,l_distance,l_duration2,l_distance2,l_duration3,l_distance3;

  //Temporary variables to compare distance if changed
  long l_tmp_distance,l_tmp_distance2,l_tmp_distance3;

         //
         // Step 1: Reserve memory space
         //
         StaticJsonBuffer<200> jsonBuffer;

         //
         // Step 2: Build object tree in memory
         //
         JsonObject& msg = jsonBuffer.createObject();


   
  //l_tmp_distance = 0;
  //l_tmp_distance2 = 0;
  //l_tmp_distance3 = 0;

 
  
  //Distance form the Ultrasonic Sensor 1
  //Generate a high pulse on Trigger Pin with 10 micro seconds delay and wait for the echo
  digitalWrite(g_trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(g_trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(g_trigPin, LOW);
  //Once received the Echo calculate the distance from the duration
  l_duration = pulseIn(g_echoPin, HIGH);
  l_distance = (l_duration/2) / 29.1;
  delay(300);
  
  //Distance form the Ultrasonic Sensor 2
  digitalWrite(g_trigPin2, LOW);
  delayMicroseconds(2); 
  digitalWrite(g_trigPin2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(g_trigPin2, LOW);
  l_duration2 = pulseIn(g_echoPin2, HIGH);
  l_distance2 = (l_duration2/2) / 29.1;
  delay(300);
  
  //Distance form the Ultrasonic Sensor 3
  digitalWrite(g_trigPin3, LOW);
  delayMicroseconds(2); 
  digitalWrite(g_trigPin3, HIGH);
  delayMicroseconds(10); 
  digitalWrite(g_trigPin3, LOW);
  l_duration3 = pulseIn(g_echoPin3, HIGH);
  l_distance3 = (l_duration3/2) / 29.1;
  delay(300);
  
  //SERIAL_PORT_HARDWARE.print("AT+send Distance from sensor1 is: ");
  //SERIAL_PORT_HARDWARE.printlnn(l_distance);
  //delay(2000);
  
  //SERIAL_PORT_HARDWARE.print("AT+send Distance from sensor2 is: ");
  //SERIAL_PORT_HARDWARE.printlnn(l_distance2);
  //delay(2000);

  //SERIAL_PORT_HARDWARE.print("AT+send Distance from sensor3 is: ");
  //SERIAL_PORT_HARDWARE.printlnn(l_distance3);
  //delay(2000);
  
  //SERIAL_PORT_HARDWARE.printlnn("\n\n\n");
  //Check if the Sensor has any fault 
  /*DATA SENT by UART to RPi is  1,2,3
    1 - Parking LOT is Free
    2 - Parking LOT is Filled
    3 - Fault in the Sensor */

  // save state
  tmp_state = state;
  // If the distance is less that g_parked, there is a car parked in the spot
  if (l_distance <= g_parked)
  {
    state = 2;
  }
  // If the distance is greater thatn g_fault, the sensor is faulting
  else if (l_distance > g_fault)
  {
    state = 3;
  }
  // The the distance is between g_parked and g_fault, assume the spot is empty
  else
  {
    state = 1;
  }

  // save state2
  tmp_state2 = state2;
  if (l_distance2 <= g_parked)
  {
    state2 = 2;
  }
  else if (l_distance2 > g_fault)
  {
    state2 = 3;
  }
  else
  {
    state2 = 1;
  }

  // save state3
  tmp_state3 = state3;
  if (l_distance3 <= g_parked)
  {
    state3 = 2;
  }
  else if (l_distance3 > g_fault)
  {
    state3 = 3;
  }
  else
  {
    state3 = 1;
  }

  
  // Updating the status of sensor 1 
  if( state != tmp_state)
  {
    msg["deviceID"] = "001";
    msg["value"] = state - 1 ;
    SERIAL_PORT_HARDWARE.println("");
    delay(50);
    SERIAL_PORT_HARDWARE.print("AT+SEND ");
    delay(50);
    msg.printTo(SERIAL_PORT_HARDWARE);
    delay(50);
    SERIAL_PORT_HARDWARE.println("");
    delay(300);
  }
   
  // Updating the status of sensor 2
  if( state2 != tmp_state2)
  {
    msg["deviceID"] = "002";
    msg["value"] = state2 - 1 ;
    SERIAL_PORT_HARDWARE.println("");
    delay(50);
    SERIAL_PORT_HARDWARE.print("AT+SEND ");
    delay(50);
    msg.printTo(SERIAL_PORT_HARDWARE);
    delay(50);
    SERIAL_PORT_HARDWARE.println("");
    delay(300);
  }
  
  // Updating the status of sensor 3
  if( state3 != tmp_state3)
  {
    msg["deviceID"] = "003";
    msg["value"]  = state3 - 1;
    SERIAL_PORT_HARDWARE.println("");
    delay(50);
    SERIAL_PORT_HARDWARE.print("AT+SEND ");
    delay(50);
    msg.printTo(SERIAL_PORT_HARDWARE);
    delay(50);
    SERIAL_PORT_HARDWARE.println("");
    delay(300);
  }
   
  
}

//End of the Program
/************************************************************************************************/