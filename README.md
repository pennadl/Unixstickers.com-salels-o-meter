## Arduino sales-o-meter | unixstickers.com

***************
sales-o-meter project by Stefano Di Luca
unixstickers.com

[See complete blog post](http://blog.unixstickers.com/2013/04/unixstickers-sales-meter-powered-by-arduino/)
***************

This script is a base sketch designed to create a sales-o-meter for our website unixstickers.com

We have used the following hardware:

- Arduino Uno
- Wi-Fi shield
- A 180° Servo
- Green and red leds to show the Wi-Fi status  
  


#### CONFIGURATION
***
**NETWORK SETTINGS**

    char ssid[] = "NETWORK SSID NAME";  // Your network SSID (name)
    char pass[] = "NETWORK PASS";       // Your network password (use for WPA, or use as key for WEP)

If your network is configured for WEP authentication change this line

    status = WiFi.begin(ssid, pass); 

TO

    status = WiFi.begin(ssid, keyIndex, pass); 




**PINS** (Change these values in order to fulfill your Arduino configuration)

    const int greenLEDPin = 2;               // Green LED connected to digital pin 3
    const int redLEDPin = 3;                 // Ref LED connected to digital pin 3
    const int servoPin = 8;                  // Servo analog pin


**HOSTNAME** (Specify an hostname or ipaddress of your website)

    IPAddress server(199,59,149,200);             // numeric IP for your website
    char server[] = "YOUR HOSTNAME";              // name address

Change these lines with your parameters

    client.println("GET /path/yourpage.php HTTP/1.1");
    client.println("Host: YOUR HOSTNAME");


**OTHER SETTINGS** (Change these settings accordingly to your needs)

    const int maxScaleValue = 400;           // Max value passed to map() function, represent the max value of your meter
    unsigned long requestInterval = 60*10000;// Delay between requests; 10 minutes
    unsigned long lastAttemptTime = 0;       // last time you connected to the server, in milliseconds
    int servoValue = 0;                      // Servo value initialized to 0
