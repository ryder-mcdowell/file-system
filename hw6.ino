#include "FS.h"
//Memset
//open has three parameters, returns int (-1 not there or where FCB lives)

void setup() {
  Serial.begin(115200);
  FS fs;

  fs.setBitOccupied(0);
  fs.setBitOccupied(1);
  fs.setBitOccupied(2);
  fs.setBitOccupied(3);

  int index = fs.findFreeBit();
  Serial.println(index);
}

void loop() {

}
