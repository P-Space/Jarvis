/**
 * Name: EMIC2
 * Author: Nick Lamprianidis (lampnick67@yahoo.com)
 * Version: 1.0
 * Description: A library for interfacing the EMIC2 Text-to-Speech module
 * License: Copyright (c) 2013 Nick Lamprianidis 
 *          This library is licensed under the MIT license
 *          http://www.opensource.org/licenses/mit-license.php
 * Source: https://github.com/pAIgn10/EMIC2
 *
 * Filename: EMIC2.cpp
 * File description: Implementation of methods for the EMIC2 library
 */

#include "EMIC2.h"

// Creates an instance of the EMIC2 class and initializes variables
EMIC2::EMIC2()
{
    _paused = 0;
    _voice = 0;
    _volume = 0;
    _rate = 200;
    _language = 0;
    _parser = 1;
    _sd = 0;
}

// Upon destruction of an instance,
// it frees dynamically allocated data members
EMIC2::~EMIC2()
{
    delete _emic2_io;
}

// Initializes serial port and checks for availability of the Emic 2 module
void EMIC2::begin(uint8_t rx_pin, uint8_t tx_pin)
{
    _emic2_io = new SoftwareSerial(rx_pin, tx_pin);
    _emic2_io->begin(9600);
    
    #ifdef VERBOSE
    Serial.println("Serial Port is set");
    #endif

    // When the Emic 2 powers on, it takes about 3 seconds for it to successfully initialize
    // It then sends a ":" character to indicate it's ready to accept commands
    // If the Emic 2 is already initialized, a CR will also causes it to send a ":"
    _emic2_io->print('\n');  // Sends a CR in case the system is already up
    while ( _emic2_io->read() != ':' ) ;  // When the Emic 2 has initialized and is ready, 
                                          // it will send a single ':' character...
                                          // so it waits here until it receives the ':'
    delay(10);  // Short delay
    _emic2_io->flush();  // Flushs receive buffer

    #ifdef VERBOSE
    Serial.println("Emic 2 Module is ready");
    #endif
     
}

// Waits (Blocking) until Emic 2 is ready to receive a command
void EMIC2::ready()
{
    _emic2_io->listen();
    _emic2_io->flush();  // Flushes receive buffer
    _emic2_io->print('\n');  // Triggers response
    while (_emic2_io->read() != ':') ;  // Awaits for ':' indicator
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(char *msg)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(msg);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(msg);
    _emic2_io->print('\n');
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(char num)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(num);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(num);
    _emic2_io->print('\n');
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(unsigned char num)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(num);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(num);
    _emic2_io->print('\n');
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(int num)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(num);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(num);
    _emic2_io->print('\n');
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(unsigned int num)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(num);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(num);
    _emic2_io->print('\n');
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(long num)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(num);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(num);
    _emic2_io->print('\n');
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(unsigned long num)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(num);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(num);
    _emic2_io->print('\n');
}

// Sends a message to the Emic 2 module to speak
void EMIC2::speak(double num)
{
    ready();

    #ifdef VERBOSE
    Serial.print("Playing message: ");
    Serial.println(num);
    #endif

    // Sends the desired string to convert to speech
    _emic2_io->print('S');
    _emic2_io->print(num);
    _emic2_io->print('\n');
}

// Pauses/Unpauses playback
EMIC2& EMIC2::operator~()
{
    _emic2_io->listen();
    // Sends pause/unpause command
    _emic2_io->print('Z');
    uint8_t val;
    while ((val = _emic2_io->read()) != '.' && val != ':') ;

    _paused = !_paused;  // Updates state

    #ifdef VERBOSE
    if ( _paused ) Serial.println("Playback unpaused");
    else Serial.println("Playback paused");
    #endif
    
    return *this;
}

// Stops playback
EMIC2& EMIC2::operator!()
{
    _emic2_io->listen();
    // Sends stop command
    _emic2_io->print('X');
    while (_emic2_io->read() != ':') ;

    #ifdef VERBOSE
    Serial.println("Playback stopped");
    #endif
    
    return *this;
}

// Raises volume by 1dB
EMIC2& EMIC2::operator++()
{
    setVolume(_volume + 1);

    #ifdef VERBOSE
    Serial.println("Volume raised");
    #endif

    return *this;
}

// Lowers volume by 1dB
EMIC2& EMIC2::operator--()
{
    setVolume(_volume - 1);

    #ifdef VERBOSE
    Serial.println("Volume lowered");
    #endif

    return *this;
}

// Raises volume by the specified dB
EMIC2& EMIC2::operator+=(uint8_t adjust_volume)
{
    setVolume(_volume + adjust_volume);

    #ifdef VERBOSE
    Serial.println("Volume raised");
    #endif

    return *this;
}

// Lowers volume by the specified dB
EMIC2& EMIC2::operator-=(uint8_t adjust_volume)
{
    setVolume(_volume - adjust_volume);

    #ifdef VERBOSE
    Serial.println("Volume lowered");
    #endif

    return *this;
}

// Increases speaking rate
EMIC2& EMIC2::operator>>(uint16_t adjust_rate)
{
    setRate(_rate + adjust_rate);

    #ifdef VERBOSE
    Serial.println("Rate increased");
    #endif

    return *this;
}

// Decreases speaking rate
EMIC2& EMIC2::operator<<(uint16_t adjust_rate)
{
    // Ensures that the result rate won't be negative...
    // ! Input argument to setRate is unsigned
    int16_t tmp_rate = _rate - adjust_rate;
    if ( tmp_rate < 0 ) tmp_rate = 0;

    setRate((uint16_t)tmp_rate);

    #ifdef VERBOSE
    Serial.println("Rate decreased");
    #endif

    return *this;
}

// Sets voice
void EMIC2::setVoice(uint8_t voice)
{
    if ( voice >= 0 && voice < 9 )
    {
        ready();

        _voice = voice;  // Updates state

        // Sends voice command
        _emic2_io->print('N');
        _emic2_io->print(_voice);
        _emic2_io->print('\n');
        while (_emic2_io->read() != ':') ;

        #ifdef VERBOSE
        Serial.print("Voice set to ");
        Serial.println(_voice);
        #endif
    }
}

// Sets volume level (in dB)
void EMIC2::setVolume(int8_t volume)
{
    ready();

    // Enforces bounds and updates state
    if ( volume < -48 ) _volume = -48;
    else if ( volume > 18 ) _volume = 18;
    else _volume = volume;

    // Sends volume command
    _emic2_io->print('V');
    _emic2_io->print(_volume);
    _emic2_io->print('\n');
    while (_emic2_io->read() != ':') ;

    #ifdef VERBOSE
    Serial.print("Volume set to ");
    Serial.println(_volume);
    #endif
}

// Resets volume level (0dB)
void EMIC2::resetVolume()
{
    setVolume(0);

    #ifdef VERBOSE
    Serial.println("Volume reset");
    #endif
}

// Sets speaking rate (in words/minute)
void EMIC2::setRate(uint16_t rate)
{
    ready();

    // Enforces bounds and updates state
    if ( rate < 75 ) _rate = 75;
    else if ( rate > 600 ) _rate = 600;
    else _rate = rate;

    // Sends rate command
    _emic2_io->print('W');
    _emic2_io->print(_rate);
    _emic2_io->print('\n');
    while (_emic2_io->read() != ':') ;

    #ifdef VERBOSE
    Serial.print("Rate set to ");
    Serial.println(_rate);
    #endif
}

// Resets speaking rate (200words/minute)
void EMIC2::resetRate()
{
    setRate(200);

    #ifdef VERBOSE
    Serial.println("Rate reset");
    #endif
}
