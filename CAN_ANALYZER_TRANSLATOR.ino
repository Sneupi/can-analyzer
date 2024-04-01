
/*
Analyzer-Translator tool for decoding automotive CAN bus

NOTE: please read TranslateCAN.h (tuning the tool to decode your vehicle)
NOTE: please set CAN_RX & CAN_TX according to your CAN transciever wiring

Hardware req: ESP32 with CAN Transciever
Software req: Terminal emulator (such as Tera Term) that supports ANSI escape codes

Serial commands list:
//  m - switch to live CAN map
//  t - switch to live CAN translations

*/

#include "CAN.h"
#include "TranslateCAN.h"
#include "TranslationHolder.h"
#include "HashMap.h"
#include "ConvertString.h"
#include "Terminal.h"

#define CAN_RX 4
#define CAN_TX 5

#define REFRESH_INTERVAL 500 //ms

CAN can = CAN();
HashMap hashmap;
TranslationHolder holder = TranslationHolder();
Terminal terminal = Terminal(&hashmap, &holder, &can);

twai_message_t can_frame;
unsigned long last_refresh = 0;

void setup() {

    Serial.begin(115200);
    while(!Serial){};

    //initialize CAN
    if (!can.init(CAN_RX, CAN_TX, 500)) {
      Serial.println("CAN init fail");
      while(1);
    }
    Serial.println("CAN init ok! - CAN_ANALYZER_TRANSLATOR");
    delay(2000);

}

void loop() {
  

  //Check for command
  //FIXME - inconsistent input behavior
  if (Serial.available()) {
    char cmd[10];
    Serial.readBytesUntil('\n', cmd, 10); //read
    terminal.processCommand(cmd); //process cmd
  }

  //Check for CAN
  if (can.avaliable() && can.get(&can_frame)) {
    insertCANtoHashMap(&hashmap, &can_frame); // update map
    holder.update(can_frame);                 // update translations
  }

  //See if time to refresh
  if (millis() - last_refresh >= REFRESH_INTERVAL) {
    terminal.refresh();
    last_refresh = millis();
  }

}



void insertCANtoHashMap(HashMap* h, twai_message_t* c) {
  //convert values to strings for hashmap
  char strID[5];
  char strBytes[35];
  ConvertString::canId(strID, c->identifier);
  ConvertString::canBytes(strBytes, c->data, c->data_length_code);

  //add new frame to hashmap as strings
  h->insert(strID, strBytes);
}
  


// END FILE
