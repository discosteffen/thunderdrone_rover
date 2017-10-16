#include <iostream>
#include <fstream>
#include <sstream>
using namespace::std;
int convert(string s) {
    int rows, cols, size, greylevels;
    string filetype;

    // open stream in binary mode
    ifstream istr(s, ios::in | ios::binary);
    if(istr.fail()) return -1;;
    // parse header
    istr >> filetype >> rows >> cols >> greylevels;
    size = rows * cols;

    // check data
    cout << "filetype: " << filetype << endl;
    cout << "rows: " << rows << endl;
    cout << "cols: " << cols << endl;
    cout << "greylevels: " << greylevels << endl;
    cout << "size: " << size << endl;

    // parse data values
    int* data = new int[size];
    int fail_tracker = 0; // find which pixel failing on
    for(int* ptr = data; ptr < data+size; ptr++) {
       
        // read in binary char
        unsigned char t_ch = istr.get();
        // convert to integer
        int t_data = static_cast<int>(t_ch);
        // check if legal pixel
        if(t_data < 0 || t_data > greylevels) { 
            cout << "Failed on pixel: " << fail_tracker << endl;
            cout << "Pixel value: " << t_data << endl;
          return -1;
        }
        // if passes add value to data array
        *ptr = t_data;
        fail_tracker++;
    }
    // close the stream
    istr.close();

    // write a new P2 binary ascii image
    ofstream ostr("new.pgm");
    // write header
    ostr << "P2 \n" << rows <<" "<< cols <<"\n"<< greylevels << endl;
    // write data
    int line_ctr = 0;
    for(int* ptr = data; ptr < data+size; ptr++) {
        // print pixel value
        ostr << *ptr << " ";
        // endl every ~20 pixels for some readability
        if(++line_ctr % 20 == 0) ostr << endl;
    }
    ostr.close();

    // clean up
    delete [] data;
    return 0;
}
