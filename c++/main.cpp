#include <iostream>
#include <vector>
#include "convert.hpp"
#include <stdlib.h>
#include <unistd.h>

int main(){
  const string filename= "old.pgm";
  while(1){
    system("rosrun map_server map_saver -f old"); //run the robot os command to update image
    convert(filename);                            //convert the binary pgm to ascii pgm
    usleep(1000000);  
    cout<<"one iteration down";
  }
  return 0;
}
