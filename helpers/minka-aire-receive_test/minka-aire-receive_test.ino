#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>
#include <ESP8266WiFi.h>

#ifdef ESP32 // for esp32! Receiver on GPIO pin 4. Transmit on GPIO pin 2.
  #define RX_PIN 4 
  #define TX_PIN 2
#elif ESP8266  // for esp8266! Receiver on pin 4 = D2. Transmit on pin 5 = D1.
  #define RX_PIN 4
  #define TX_PIN 5
#else // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on pin 6.
  #define RX_PIN 0
  #define TX_PIN 6
#endif 

// Set CC1101 frequency
// 303.808 determined from FCC Filing https://fccid.io/2AHC3CR400T
#define FREQUENCY     303.808

int long value;      // int to save value
int bits;           // int to save bit number
int prot;          // int to save Protocol number

RCSwitch mySwitch = RCSwitch();

void printBinaryWithLeadingZeros(unsigned long num) {
  for (int i = bits - 1; i >= 0; i--) {
    Serial.print((num & (1ul << i)) ? '1' : '0');
  }
}

void setup() {
  Serial.begin(9600);
  
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setMHZ(FREQUENCY);
  ELECHOUSE_cc1101.SetRx();
  mySwitch.enableReceive(RX_PIN);
}

void loop() {
    if (mySwitch.available()) {
    value =  mySwitch.getReceivedValue();        // save received Value
    prot = mySwitch.getReceivedProtocol();     // save received Protocol
    bits = mySwitch.getReceivedBitlength();     // save received Bitlength

    Serial.print(prot);
    Serial.print(" - ");
    printBinaryWithLeadingZeros(value);
    Serial.print(" - ");
    Serial.println(bits);


    
    mySwitch.resetAvailable();
    }


}
