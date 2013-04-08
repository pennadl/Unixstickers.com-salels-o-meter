/*
Developed with Arduino, by Stefano Di Luca.
Unixstickers.com

Unixstickers.com sales-o-meter
*/

#include <SPI.h>
#include <WiFi.h>
#include <Servo.h>

// Initialize the library instance:
WiFiClient client;

// Create a servo object 
Servo myServo;


//** WIFI CONFIGURATION
char ssid[] = "NETWORK SSID NAME";       // your network SSID (name) 
char pass[] = "NETWORK PASS";            // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                        // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;             // Initital status of the wifi connection


//** HOSTNAME / IP CONFIGURATION
// If you don't want to use DNS (and reduce your sketch size)
// Use the numeric IP instead of the name for the server:
//IPAddress server(ipaddress);           // numeric IP for your website
char server[] = "YOUR HOSTNAME";         // name address


//** CONFIGURE PINS
const int greenLEDPin = 2;               // Green LED connected to digital pin 3
const int redLEDPin = 3;                 // Red LED connected to digital pin 3
const int servoPin = 8;                  // Servo pin


//** OTHER CONFIGURATIONS
const int maxScaleValue = 400;           // Max value passed to map() function, represent the max value of your meter
boolean requested;                       // whether you've made a request since connecting
boolean readingTotal = false;            // if you're currently reading the total
boolean finded = false;                  // Control the beginning of reading total
unsigned long requestInterval = 60*10000;// Delay between requests; 10 minutes
unsigned long lastAttemptTime = 0;       // last time you connected to the server, in milliseconds
int servoValue = 0;


//** INITIALIZE STRINGS
String currentLine = "";                 // string to hold the text from server
String tot_string = "";                  // string to hold the total

void setup() {
 
  // reserve space for the strings:
  currentLine.reserve(256);
  tot_string.reserve(256);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 

  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(servoValue); //Initialise servo position to 0 degrees
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  digitalWrite(redLEDPin, HIGH);  // Always turon on red led on start

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  }
   
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);  
 
    // wait 10 seconds for connection:
    delay(10000);
  } 
  
  // you're connected now, so print out the status:
  printWifiStatus();
  connectToServer();
}

void loop()
{
  
  if(WiFi.status() != WL_CONNECTED){
      digitalWrite(greenLEDPin, LOW);
      digitalWrite(redLEDPin, HIGH);
  } else {
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
  }
    
  if (client.connected()) {
    if (client.available()) {
      lastAttemptTime = millis();
      long requestInterval = 60 * 10000;   

      char inChar = client.read();

      // add incoming byte to end of line:
      currentLine += inChar; 

      // if you get a newline, clear the line:
      if (inChar == '\n') {
        //Serial.print("Current Line: ");
        //Serial.println(currentLine);
        currentLine = "";
      } 
      
      // if the current line ends with <total>, it will
      // be followed by the total:
      if (currentLine.endsWith("<total>")) {
        // Total is beginning. Clear tot_string:
        readingTotal = true; 
        tot_string = "";
        // break out of the loop so this character isn't added to the total:
        return;
      }
      
      // if you're currently reading the bytes of the total, add them to tot_string:
      if (readingTotal) {
        if (inChar != '<'){
          if(inChar != ' '){
            tot_string += inChar;
          }
        } else {
          readingTotal = false;
          finded = true;
        }
      } 
      
      if (finded) {
          Serial.println(tot_string);
          int tot = Str2int(tot_string);
          Serial.println(tot);
          
          if(servoValue != map(tot, maxScaleValue, 0 , 179 , 0)){
            servoValue = map(tot, maxScaleValue, 0 , 179 , 0);
            if(servoValue > 179)
              servoValue = 179;
            Serial.println(servoValue);
            myServo.write(servoValue);
          } else {
            Serial.println("No changes, Servo in correct position.");
          }
          
          tot_string = "";
          finded = false;  
          // close the connection to the server:
          client.stop(); 
        }
    }   
  }
  else if (millis() - lastAttemptTime > requestInterval) {
     /*
     Serial.print("Reconnect.. millis: ");
     Serial.print(millis());
     Serial.print(" - lastAttemptTime: ");
     Serial.print(lastAttemptTime);
     Serial.print(" - requestInterval: ");
     Serial.println(requestInterval);
     */
     
    // if you're not connected, and ten minutes have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
}

void connectToServer() {
  // attempt to connect, and wait a millisecond:
  Serial.println("connecting to server...");
  if (client.connect(server, 80)) {
    Serial.println("making HTTP request...");
    // make HTTP GET request to server:
    client.println("GET /path/yourpage.php HTTP/1.1");
    client.println("Host: YOUR HOSTNAME");
    client.println("Connection:close");
    client.println();
  }
  
}   

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.println(WiFi.status());
}

int Str2int (String Str_value)
{
  char buffer[8]; //max length is three units
  Str_value.toCharArray(buffer, 8);
  int int_value = atoi(buffer);
  return int_value;
} 
