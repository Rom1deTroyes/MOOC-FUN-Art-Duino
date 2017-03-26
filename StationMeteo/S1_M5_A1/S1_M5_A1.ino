

/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "infoclimat.fr";    // name address for Google (using DNS)

///Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

TextFinder finder_temp( client );

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("hello");

  // start the Ethernet connection:
if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
 
  //Ethernet.begin(mac, ip);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET http://www.infoclimat.fr/public-api/gfs/xml?_ll=48.85341,2.3488&_auth=U0kCFQJ8VnQCLwYxBXMAKQRsAzYPeVB3B3sKaQFkVSgAa1c2VjZSNAVrUy5VegM1VntTMAswBDRTOAtzXS9QMVM5Am4CaVYxAm0GYwUqACsEKgNiDy9QdwdsCmsBclU3AGpXO1YrUjcFbFMvVWcDNlZiUywLKwQ9UzQLZF02UDJTMwJkAmJWNQJsBnsFKgAxBD8Dag80UGEHYApoAWtVZwBjV2VWNVIwBWhTL1VgAz5WZ1M6CzEEPlMzC21dL1AsU0kCFQJ8VnQCLwYxBXMAKQRiAz0PZA%3D%3D&_c=d1bfb7a6d663a64de4e2a86e2357e929 HTTP/1.1");
   
    client.println("Host: infoclimat.fr");
    client.println("Connection: close");
    client.println();

    float TK;
    float TC;
    

if(finder_temp.find("<echeance hour=\"24\" ")){
  if(finder_temp.find("<temperature>")){
    if(finder_temp.find("<level val=\"2m\">")){
      float TK=finder_temp.getFloat();
      TC=TK-273,15;
      //int a = round(TC);
      Serial.print("TK : ");Serial.println(TK);
      Serial.print("TC : ");Serial.println(TC);
    }
}
}


    
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() {
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}

