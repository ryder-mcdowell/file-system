#include "FS.h"

FS::FS():eeprom() {
  
}

void FS::initialize() {
  //bring in bit vector
  eeprom.read_page(0, free_space_list);
  //bring in directory
  eeprom.read_page(1, (byte *) directory_table);
}

//overwrite free space list and directory table to open values and write to chip
void FS::reformat() {
  Serial << "formatting EEPROM..." << endl;
  //write over current lists to initial values
  for (int i = 0; i < BLOCK_SIZE; i++) {
    free_space_list[i] = 0xFF;
    if (i < BLOCK_SIZE / 2) {
      directory_table[i] = -1;
    }
  }
  
  //set first two blocks as taken (bit vector and directory)
  setBitOccupied(0);
  setBitOccupied(1);
    
  //write to backing store
  eeprom.write_page(0, free_space_list);
  eeprom.write_page(1, (byte *) directory_table);
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
  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < MASKS_SIZE; j++) {
      if ((free_space_list[i] & masks[j]) == masks[j]) {
        return ((i * 8) + j);
      }
    }
  }
  //no free bits
  return -1;
}

int FS::findFreeDirectoryLocation() {
  //scan for -1, return index
  for (int i = 0; i < BLOCK_SIZE / 2; i++) {
    if (directory_table[i] == -1) {
      return i;
    }
  }
  //no free location
  return -1;
}

//list all files in file directory
void FS::list_files() {
  Serial << "listing files..." << endl;
  FCB fcb;
  for (int i = 0; i < BLOCK_SIZE / 2; i++) {
    if (directory_table[i] != -1) {
      eeprom.read_page(directory_table[i], (byte *) &fcb);
      Serial << "File: " << fcb.filename << ", _ bytes" << endl;
    }
  }
}

//find and return index of given file
int FS::find_file(char *filename) {
  FCB fcb;
  for (int i = 0; i < BLOCK_SIZE / 2; i++) {
    if (directory_table[i] != -1) {
      eeprom.read_page(directory_table[i], (byte *) &fcb);
      if (strcmp(fcb.filename,filename) == 0) {
        return i;
      }
    }
  }
  return -1;
}

//create file w/ name
int FS::create_file(char *filename) {
  //check if filename is too large
  if (strlen(filename) > 30) {
    Serial << "create_file ERROR: filename must be less than 31 characters" << endl;
    return -1;
  }
  
  //check if file exists
  if (find_file(filename) != -1) {
    Serial << "create_file ERROR: file already exists" << endl;
    return -1;
  }
  
  //find available spot(if any)
  int free_bit_index = findFreeBit();
  if (free_bit_index == -1) {
    Serial << "create_file ERROR: no space in memory" << endl;
    return -1;
  }

  //find available spot(if any)
  int free_directory_index = findFreeDirectoryLocation();
  if (free_directory_index == -1) {
    Serial << "create_file ERROR: no space in FCB directory" << endl;
    return -1;
  }

  //update bit vector
  setBitOccupied(free_bit_index);
  //update directory
  directory_table[free_directory_index] = free_bit_index;

  //create empty FCB
  FCB fcb;
  strcpy(fcb.filename, filename);
  fcb.offset = 0;
  for (int i = 0; i < FCB_DATA_BLOCKS_COUNT; i++) {
    fcb.data_blocks[i] = -1;
  }

  //write to backing store
  eeprom.write_page(0, free_space_list);
  eeprom.write_page(1, (byte *) directory_table);
  eeprom.write_page(free_bit_index, (byte *) &fcb);

  //return index to block
  return free_bit_index;
}

int FS::delete_file(char *filename) {
  //check if filename is too large
  if (strlen(filename) > 30) {
    Serial << "delete_file ERROR: filename must be less than 31 characters" << endl;
    return -1;
  }

  FCB fcb;
  //search for matching filename
  for (int i = 0; i < BLOCK_SIZE / 2; i++) {
    if (directory_table[i] != -1) {
      eeprom.read_page(directory_table[i], (byte *) &fcb);
      if (strcmp(filename, fcb.filename) == 0) {
        //mark free
        setBitFree(directory_table[i]);
        directory_table[i] = -1;

        //write to backing store
        eeprom.write_page(0, free_space_list);
        eeprom.write_page(1, (byte *) directory_table);
      }
    }
  }
  //file not found
  return -1;
}

int FS::open_file(char *filename) {
  int directory_index = find_file(filename);
  if (directory_index == -1) {
    Serial << "open_file ERROR: file not found" << endl;
    return -1;
  }
  Serial << directory_index << endl;
  //return address to memory
}

