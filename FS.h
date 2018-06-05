#pragma once
#include <Arduino.h>
#include <Streaming.h>
#include <Microchip_24LC256.h>

#define FREE_SPACE_SIZE 64
#define DIRECTORY_SIZE 32
#define MASKS_SIZE 8

class FS {
  public:
    FS();
    void initialize();
    void setBitOccupied(int bit_index);
    void setBitFree(int bit_index);
    int findFreeBit();

  private:
    byte free_space_list[FREE_SPACE_SIZE];
    int directory_table[DIRECTORY_SIZE];
    byte masks[MASKS_SIZE] = { 0x01, 0x02, 0x04, 0x08, 0x010, 0x20, 0x40, 0x80 };
};

