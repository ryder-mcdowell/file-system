#include "FS.h"

FS::FS() {
  for (int i = 0; i < DIRECTORY_SIZE; i++) {
    directory_table[i] = -1;
  }
  for (int i = 0; i < FREE_SPACE_SIZE; i++) {
    free_space_list[i] = 0xFF;
  }
}

void FS::initialize() {
  //bring in bit vector
  //bring in directory
}

//set bit at index to be occupied(0)
void FS::setBitOccupied(int bit_index) {
  int number = bit_index / 8;
  int offset = bit_index % 8;

  byte mask = masks[offset];

  mask = ~mask;
  free_space_list[number] &= mask;
}

//set bit at index to be free(1)
void FS::setBitFree(int bit_index) {
  int number = bit_index / 8;
  int offset = bit_index % 8;

  byte mask = masks[offset];

  free_space_list[number] ^= mask;
}

//return index of free bit or -1 if none found
int FS::findFreeBit() {
  //for each bit in byte, check if free using masks
  for (int i = 0; i < FREE_SPACE_SIZE; i++) {
    for (int j = 0; j < MASKS_SIZE; j++) {
      if ((free_space_list[i] & masks[j]) == masks[j]) {
        return (i + j);
      }
    }
  }
  //no free bits
  return -1;
}

