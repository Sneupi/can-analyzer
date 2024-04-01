
#ifndef CONVERT_STRING_H 
#define CONVERT_STRING_H

#include <Arduino.h>

/*
Custom methods specifically-for translating CAN frame
IDs and byte arrays into proper hex strings
*/

class ConvertString {
private:
  /*none*/
public:

  //uint32_t CAN ID to hex string
  static void canId(char* ret, uint32_t val) {
    sprintf(ret, "%03X", val);
  }

  //uint8_t CAN byte array to hex string delimit "  "
  static void canBytes(char* ret, uint8_t* vals, uint8_t len) {
    if (len > 0) { sprintf(ret, "%02X", vals[0]); }
    for (int i = 1; i < len; i++) {
      char temp[7];
      sprintf(temp, "  %02X", vals[i]);
      strcat(ret, temp);
    }
  }
};

#endif // CONVERT_STRING_H