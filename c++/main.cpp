#include <iostream>
#include <vector>
#include "convert.hpp"
#include <stdlib.h>
#include <unistd.h>

int main(){
  const string filename= "old.pgm";
  long count=0;
  while(1){
    system("rosrun map_server map_saver -f old"); //run the robot os command to update image
    convert(filename);                            //convert the binary pgm to ascii pgm
    usleep(1000000);
  }
  return 0;
}
