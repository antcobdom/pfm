/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <printf.h>

// nRF24L01(+) radio attached to SPI and pins 7 & 8
RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

void setup(void)
{

  Serial.begin(57600);
  printf_begin();
  Serial.println("RF24Network/examples/helloworld_rx/");
  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 107, /*node address*/ this_node);
  radio.printDetails();
}

void loop(void)
{


  // Pump the network regularly
  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    char message;
    network.read(header,&message,sizeof(message));
    Serial.print("Node: ");
    Serial.println(header.from_node);
    Serial.print("Header: ");
    Serial.println(header.type);
    Serial.print("Received: ");
    Serial.println(message);
  }

  delay(1000);
}
