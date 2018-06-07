#pragma once
#include <Arduino.h>
#include <Streaming.h>
#include <Microchip_24LC256.h>

#define BLOCK_SIZE 64
#define MASKS_SIZE 8
#define MAX_FILENAME_SIZE 30
#define FCB_DATA_BLOCKS_COUNT 16

struct FCB {
  char filename[MAX_FILENAME_SIZE];
  int offset;
  int data_blocks[FCB_DATA_BLOCKS_COUNT];
};

class FS {
  public:
    FS();
    void initialize();
    void reformat();
    void list_files();
    int create_file(char *filename);
    int delete_file(char *filename);
    int open_file(char *filename);

  private:
    Microchip_24LC256 eeprom;
    byte free_space_list[BLOCK_SIZE];
    int directory_table[BLOCK_SIZE / 2];
    byte masks[MASKS_SIZE] = { 0x01, 0x02, 0x04, 0x08, 0x010, 0x20, 0x40, 0x80 };
    int findFreeDirectoryLocation();
    int findFreeBit();
    int find_file(char *filename);
    void setBitOccupied(int bit_index);
    void setBitFree(int bit_index);
};

