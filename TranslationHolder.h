/*
Wrapper for conveniently holding onto a 
collection of values translatable by
TranslateCAN class
*/


#ifndef TRANSLATION_HOLDER_H 
#define TRANSLATION_HOLDER_H

#include <Arduino.h>
#include "driver/twai.h"
#include "TranslateCAN.h"

//----- TranslationHolder Class ------------------------------------------------------

class TranslationHolder {
private:

	uint16_t rpm;
  char     gear[3];
  uint16_t speed;   
  uint16_t oilTemp;
  uint16_t oilPressure;
  uint16_t coolantTemp;
  uint16_t fuelPSI;
  uint16_t fuelPercent;
  float    batteryVolt;
  float    afr;

public:

  TranslationHolder() {
    reset();
  }
	
  // Parses CAN frame according to TranslateCAN 
  // and updates respective attributes/variables
  void update(twai_message_t c) {

    uint32_t id = c.identifier;

    uint8_t bytes[8] = {0,0,0,0,0,0,0,0};
    for (int i = 0; i < c.data_length_code; i++) {
      bytes[i] = c.data[i];
    }

    if (id == CAN_RPM)         rpm         = TranslateCAN::rpm(bytes);
    if (id == CAN_GEAR)        strcpy(gear,  TranslateCAN::gear(bytes));
    if (id == CAN_SPEED)       speed       = TranslateCAN::speed(bytes);
    if (id == CAN_OILTEMP)     oilTemp     = TranslateCAN::oilTemp(bytes);
    if (id == CAN_OILPRESSURE) oilPressure = TranslateCAN::oilPressure(bytes);
    if (id == CAN_COOLANTTEMP) coolantTemp = TranslateCAN::coolantTemp(bytes);
    if (id == CAN_FUELPSI)     fuelPSI     = TranslateCAN::fuelPSI(bytes);
    if (id == CAN_FUELPERCENT) fuelPercent = TranslateCAN::fuelPercent(bytes);
    if (id == CAN_BATTERYVOLT) batteryVolt = TranslateCAN::batteryVolt(bytes);
    if (id == CAN_AFR)         afr         = TranslateCAN::afr(bytes);

  }

  // Prints comprehensive list of values in holder, 
  // and their associated CAN IDs
  void print() {

    Serial.println("    NAME    |   ID  | TRANSLATION");
    Serial.println("==================================");
    Serial.printf ("    rpm     | %5X | %u \n",   CAN_RPM,         rpm);
    Serial.printf ("    gear    | %5X | %s \n",   CAN_GEAR,        gear);
    Serial.printf ("   speed    | %5X | %u \n",   CAN_SPEED,       speed);
    Serial.printf ("  oilTemp   | %5X | %u \n",   CAN_OILTEMP,     oilTemp);
    Serial.printf ("oilPressure | %5X | %u \n",   CAN_OILPRESSURE, oilPressure);
    Serial.printf ("coolantTemp | %5X | %u \n",   CAN_COOLANTTEMP, coolantTemp);
    Serial.printf ("  fuelPSI   | %5X | %u \n",   CAN_FUELPSI,     fuelPSI);
    Serial.printf ("fuelPercent | %5X | %u \n",   CAN_FUELPERCENT, fuelPercent);
    Serial.printf ("batteryVolt | %5X | %.1f \n", CAN_BATTERYVOLT, batteryVolt);
    Serial.printf ("    afr     | %5X | %.1f \n", CAN_AFR,         afr);
  }

  // reset all values to zero, NULL or equivalent
  void reset() {
    rpm          = 0;
    gear[0] = '-'; gear[1] = '\0';
    speed        = 0;   
    oilTemp      = 0;
    oilPressure  = 0;
    coolantTemp  = 0;
    fuelPSI      = 0;
    fuelPercent  = 0;
    batteryVolt  = 0.0;
    afr          = 0.0;
  }
	
};


#endif // TRANSLATION_HOLDER_H