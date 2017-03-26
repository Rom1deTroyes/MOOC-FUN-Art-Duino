

/*
  Repeating Web client
 
 This sketch connects to a a web server and makes a request
 using a Wiznet Ethernet shield. You can use the Arduino Ethernet shield, or
 the Adafruit Ethernet shield, either one will work, as long as it's got
 a Wiznet Ethernet module on board.
 
 This example uses DNS, by assigning the Ethernet client with a MAC address,
 IP address, and DNS address.
 
 Circuit:
 Ethernet shield attached to pins 10, 11, 12, 13
 
 created 19 Apr 2012
 by Tom Igoe
 modified 21 Jan 2014
 by Federico Vanzati
 
 http://www.arduino.cc/en/Tutorial/WebClientRepeating
 This code is in the public domain.
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>
#include <Servo.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// fill in an available IP address on your network here,
// for manual configuration:
//IPAddress ip(192, 168, 1, 177);

// fill in your Domain Name Server address here:
//IPAddress myDns(1, 1, 1, 1);

// initialize the library instance:
EthernetClient client;
TextFinder finder( client );

Servo servoM;

#define POS0 600
#define POS45 1050
#define POS90 1542
#define POS135 2000

int ledR = 3;
int ledV = 4;
int ledB = 5;

int cdeServo = 9;

boolean firstConnection = 1;

boolean x = 0;
char server[] = "infoclimat.fr";

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 20L*1000L; // delay between updates, in milliseconds : 1 heure
// the "L" is needed to use long type numbers

void setup() {
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(ledB, OUTPUT);
  Serial.begin(9600);

  Ethernet.begin(mac);
  delay(1000);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  servoM.attach(cdeServo);
  delay(200);
  servoM.writeMicroseconds(POS0);
}

void loop() {
  if (!firstConnection) {
    if (millis() - lastConnectionTime > postingInterval) {
      Serial.println("autre connexion");
      httpRequest();
    }

  } 
  else {
    Serial.println("first connexion");
    httpRequest();
  }

}

void httpRequest() {

  client.stop();
  float TK;
  float TC;
  float MM;
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // http://www.infoclimat.fr/previsions-meteo/details/2986692/ploudalmezeau.html
    client.println("GET http://www.infoclimat.fr/public-api/gfs/xml?_ll=48.53333,-4.65&_auth=VE5RRgN9VHZfcgM0UyUDKlU9UGUKfAUiCnZWNQxpVyoAZlM0VjYGZ1U5BnsGKQQtUWYGeAsyCTQLdwR8DmFTOVQ%2FUTMDZlQ%2FXzIDflN8AypVZ1AvCioFOgphVi8MY1c1AGdTKVY9BmBVJAZgBjAEL1F9BmcLMAk4C2gEZg5gUzhUN1EyA2ZUKV8vA2RTawNjVWFQYwpgBTgKOlYwDGRXNQAxUzVWPAZ6VT0GYgYxBDFRZAZjCzYJMwt3BHwOGlNDVCpRdQMiVGNfdgN8UzYDaVUy&_c=df2892ebed651e129245c41a7ee25040 HTTP/1.1");
    client.println("Host: infoclimat.fr");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();



    if (finder.find("<echeance hour=\"3\" ")) {
      if (finder.find("<temperature>")) {
        if (finder.find("<level val=\"2m\">")) {
          TK = finder.getFloat(' ');
          Serial.print("TK : "); 
          Serial.println(TK);
          TC = TK - 273,15;
        }
      }
    }

    if (finder.find("<pluie interval=\"3h\">")) {
      MM = finder.getFloat(' ');
    }

    client.stop();


  } 
  else {
    Serial.println("connection failed");
    TC = random(0,30);
    MM = random(0,7);
  }



  firstConnection = 0;
  afficheAlarme(TC);
  afficheMeteo(TC,MM);
}

void afficheMeteo(float a, float b) {

  Serial.print("TC : "); 
  Serial.println(a);
  Serial.print("MM : "); 
  Serial.println(b);

  if(b < 6){
    if(a > 20){
      Serial.println("lunettes et pieds nus");
      servoM.writeMicroseconds(POS0); //0
    }
    if(a>5 && a<=20){
      Serial.println("Chaussures de ville");
      servoM.writeMicroseconds(POS45); // 45 degré - tempéré et pas de pluie
    }
    if(a <= 5){
      Serial.println("Bonnet et moom boots");
      servoM.writeMicroseconds(POS135); //135 degré
    }
  }
  else{
    Serial.println("capuche et bottes");
    servoM.writeMicroseconds(POS90); //90 degré Pluie
  }

}


void afficheAlarme(float a) {

  if(a >= 25){
    digitalWrite(ledR,HIGH);
    digitalWrite(ledV,LOW);
    digitalWrite(ledB,LOW);
  }
  if(a > 0 && a < 25){
    digitalWrite(ledR,LOW);
    digitalWrite(ledV,HIGH);
    digitalWrite(ledB,LOW);
  }
  if(a <= 0){
    digitalWrite(ledR,LOW);
    digitalWrite(ledV,LOW);
    digitalWrite(ledB,HIGH);
  }

}



