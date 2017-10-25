#include <fstream>
#include <vector>
#include <iostream>

using namespace std;
int main(){
  vector <string> color;
  int count = 0;
  string word;
  ifstream infile;
  infile.open("new.pgm");
  while (infile >> word)
  {
    if(color.size()==0){
      color.push_back(word);
      continue;
    }
    for(int i = 0; i< color.size(); i++){
      if(word == color[i]){
        count = 1;
        break;
      }
    }
    if(count ==0)
      color.push_back(word);

    count = 0;
  }
  for (int i=3; i<color.size(); i++)
    cout << color[i] << "\n";
  return 0;
}
