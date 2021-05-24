#include <iostream>
using namespace std;


// takes data of the location and status of elevators as dynamic 2d array 
void graphic(int **GFX, int Columns, int Rows) {  
  for (int i = 0; i < Columns; i++) {
    for (int j = 0; j < Rows; j++) {
      if (j == 0 && (i < (Columns - 3))) {
        cout << 'F' << (Columns - (i + 3)) << ':' << '\t';   //print the number of the floor in the first column as the the number next to F is (columns -(i+3))

      } else if (j == 0 && (i == (Columns - 3))) {
        cout << "G " << ':' << '\t';                        // print G in the first column at the ground floor 
      } else if (j == 0 && (i > (Columns - 3))) {
        cout << 'B' << -(Columns - (i + 3)) << ':' << '\t'; // print the number of basment floors in the first column too 
      }

      else {
        if (GFX[i][j] == 0) {
          cout << "|  " << '\t';                  // print '|  ' indicating there is no elevator here
        } else if (GFX[i][j] == 1) {
          cout << "|__" << '\t';                 // print '|__' indacting there is a closed elevator here
        } else if (GFX[i][j] == 2) {
          cout << "|00" << '\t';                 // print '|00' indicating there is an opened elevator here
        } else {
          cout << "   " << '\t';
        }
      }
    }
    cout << '\n';
  }
}