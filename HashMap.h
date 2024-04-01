
#ifndef HASHMAP_H 
#define HASHMAP_H

#include <Arduino.h>

#define HASH_MAP_SIZE 50
#define MAX_KEY_SIZE 6  // Including null terminator
#define MAX_VALUE_SIZE 36  // Including null terminator
#define TO_STRING_BUFFER_SIZE (HASH_MAP_SIZE * (MAX_KEY_SIZE + MAX_VALUE_SIZE + 2 + 1) + 1)

class HashMap {
  private:
    struct Entry {
      char key[MAX_KEY_SIZE];
      char value[MAX_VALUE_SIZE];
      bool occupied = false;
    };

    Entry hashMap[HASH_MAP_SIZE];

    unsigned int hash(const char* str) {
      unsigned int hash = 5381;
      int c;

      while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

      return hash % HASH_MAP_SIZE;
    }

  public:
    HashMap() {}

    void insert(const char* key, const char* value) {
      unsigned int index = hash(key);
      
      while (hashMap[index].occupied && strncmp(hashMap[index].key, key, MAX_KEY_SIZE) != 0) {
        index = (index + 1) % HASH_MAP_SIZE;
      }
      
      strncpy(hashMap[index].key, key, MAX_KEY_SIZE - 1);
      hashMap[index].key[MAX_KEY_SIZE - 1] = '\0'; // Ensure null-termination
      strncpy(hashMap[index].value, value, MAX_VALUE_SIZE - 1);
      hashMap[index].value[MAX_VALUE_SIZE - 1] = '\0'; // Ensure null-termination
      hashMap[index].occupied = true;
    }

    char* get(const char* key) {
      unsigned int index = hash(key);
      
      while (hashMap[index].occupied) {
        if (strncmp(hashMap[index].key, key, MAX_KEY_SIZE) == 0) {
          return hashMap[index].value;
        }
        index = (index + 1) % HASH_MAP_SIZE;
      }
      
      return NULL;
    }

    void toString(char* buffer, size_t bufferLength) {
      buffer[0] = '\0'; // Start with an empty string
      char entryBuffer[MAX_KEY_SIZE + MAX_VALUE_SIZE + 3]; // Extra space for ": " and "\n"
      
      for (int i = 0; i < HASH_MAP_SIZE; i++) {
        if (hashMap[i].occupied) {
          snprintf(entryBuffer, sizeof(entryBuffer), "%s: %s\n", hashMap[i].key, hashMap[i].value);
          if (strlen(buffer) + strlen(entryBuffer) < bufferLength) {
            strcat(buffer, entryBuffer);
          } else {
            // Not enough space in buffer to add more entries
            return;
          }
        }
      }
    }
};

#endif //HASHMAP_H


