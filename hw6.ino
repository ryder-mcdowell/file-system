#include "FS.h"
//open has three parameters, returns int (-1 not there or where FCB lives)

void setup() {
  Serial.begin(115200);
  FS fs;

  fs.initialize();
  fs.reformat();
  
  fs.create_file("filename.txt");
  
  fs.list_files();
  
  fs.open_file("filename.txt");
  
}

void loop() {

}
