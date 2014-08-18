/**
 * Author: Nick Lamprianidis {paign10.ln@gmail.com}
 * Description: An ATTiny85 on the door side is receiving data from an xbee
 *              and is controlling the lock's solenoid, a NeoPixel ring,
 *              and an EMIC2 module
 * Note: The EMIC2 library was modified to exclude any methods
 *       using the Serial or the SD library
 */

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include "EMIC2.h"

const uint8_t RING_DATA_PIN = 0;
const uint8_t XBEE_RX_PIN = 1;
const uint8_t XBEE_TX_PIN = 2;
const uint8_t RELAY_PIN = 2;
const uint8_t EMIC2_RX_PIN = 3;
const uint8_t EMIC2_TX_PIN = 4;

long timeRef;
char gtBuf[28];
char qtBuf[53];

Adafruit_NeoPixel ring(24, RING_DATA_PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial xbee(XBEE_RX_PIN, XBEE_TX_PIN);
EMIC2 emic;

void setup()
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    ring.begin();
    ring.setBrightness(200);
    ring.show();

    emic.begin(EMIC2_RX_PIN, EMIC2_TX_PIN);
    emic.setVolume(15);
    
    xbee.begin(9600);
}

void loop()
{
    // Blinks (red) when idle
    if ( (unsigned long) (millis() - timeRef) > 1500 )
    {
        setColor(ring.Color(255, 0, 0));
        delay(100);
        setColor(ring.Color(0, 0, 0));
        timeRef = millis();
    }


    if ( xbee.available() )
    {
        uint8_t i = 0;

        delay(50);  // Waits for the whole msg to arrive
        while ( xbee.available() ) gtBuf[i++] = xbee.read();  // Retrieves greeting msg
        gtBuf[i] = '\0';

        while ( !xbee.available() ) ;  // Blocks until next msg begins to arrive

        i = 0;
        delay(50);  // Waits for the whole msg to arrive
        while ( xbee.available() ) qtBuf[i++] = xbee.read();  // Retrieves quote msg
        qtBuf[i] = '\0';
        
        colorWipe(ring.Color(255, 0, 0));

        digitalWrite(RELAY_PIN, HIGH);  // Activates the door relay (for 2 seconds)
        timeRef = millis();

        fadeOut();
        setColor(ring.Color(0, 255, 0));
        delay(1000);
        setColor(ring.Color(0, 0, 0));

        while ( (unsigned long) (millis() - timeRef) < 2000 ) ;
        digitalWrite(RELAY_PIN, LOW);  // Deactivates door relay

        uint8_t voice = rand() % 9;
        emic.setVoice(voice);
        delay(1000);
        emic.speak(gtBuf);

        // If no quote is to be spoken a '+' character is sent
        if ( qtBuf[0] != '+' )
        {
            delay(1000);
            emic.speak(qtBuf);
        }

        xbee.listen();
    }
}

void setColor(uint32_t c)
{
    for (uint8_t i = 0; i < ring.numPixels(); ++i)
        ring.setPixelColor(i, c);
    ring.show();
}

void fadeOut()
{
    for (uint8_t c = 255; c > 0; --c)
    {
        for (uint8_t i = 0; i < ring.numPixels(); ++i)
            ring.setPixelColor(i, c, 0, 0);
        ring.show();
        delayMicroseconds(500);
    }
}

void colorWipe(uint32_t c)
{
    for(uint8_t i = 0; i < ring.numPixels(); ++i)
    {
      ring.setPixelColor(i, c);
      ring.show();
      delay(10);
    }
}
