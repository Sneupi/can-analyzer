/*
Class for managing a static windowed
terminal output for CAN Analyzer-Translator
*/


#ifndef TERMINAL_H 
#define TERMINAL_H

#include <Arduino.h>

#include "HashMap.h"
#include "TranslationHolder.h"
#include "CAN.h"

#define CONN_STATUS (can->isConnected() ? "[CONNECTED]" : "[DISCONNECTED]")

enum TerminalPage {
    PAGE_CAN_MAP,
    PAGE_TRANSLATIONS,
    PAGE_UNKNOWN_CMD
};

//----- Terminal Class ------------------------------------------------------

class Terminal {
private:

  // so refresh() knows which page Terminal is currently in
  TerminalPage output = PAGE_CAN_MAP;

  //references to externally provided data
  HashMap*           can_map;
  TranslationHolder* can_translations;
  CAN*               can;

  void print_CAN_MAP() {
    clearScreen();
    char toStringBuffer[TO_STRING_BUFFER_SIZE];
    can_map->toString(toStringBuffer, sizeof(toStringBuffer));
    Serial.print(toStringBuffer);
    Serial.println(CONN_STATUS);
  }

  void print_TRANSLATIONS() {
    clearScreen();
    can_translations->print();
    Serial.println(CONN_STATUS);
  }

  void print_UNKNOWN_CMD() {
    clearScreen();
    Serial.println("=================================================");
    Serial.println("=============    UNKNOWN COMMAND    =============");
    Serial.println("=================================================");
  }

  //reset position using ANSI escape sequences
  void clearScreen() {
    // Clear the screen
    Serial.print("\x1B[2J");
    // Move the cursor to the top-left corner
    Serial.print("\x1B[H");
  }

public:

  Terminal(HashMap* m, TranslationHolder* h, CAN* c) : 
    can_map(m), 
    can_translations(h),
    can(c) 
    {}

  void refresh() {
    if      (output == PAGE_CAN_MAP)      print_CAN_MAP();
    else if (output == PAGE_TRANSLATIONS) print_TRANSLATIONS();
    else                                  print_UNKNOWN_CMD();
  }

  // Function to process a received command (i.e. screen switching)
  void processCommand(char* cmd) {

    //  m - switch to live CAN map
    if (strcmp(cmd, "m") == 0) {
      output = PAGE_CAN_MAP;
    } 

    // t - switch to live CAN translations
    else if (strcmp(cmd, "t") == 0) {
      output = PAGE_TRANSLATIONS;
    } 

    //show page for bad command
    else {
      output = PAGE_UNKNOWN_CMD;
    }
  }

};


#endif // TERMINAL_H