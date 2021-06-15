#include<iostream>
#include<fstream>
#include<string>
#include"Client.h"
using namespace std;

int count_lines(string fileName ) 
// this funtion returns the number of lines in the txt file
{
  // create a variable of type fstream
    fstream myFile;

    // open a file in the read mode 
    myFile.open(fileName,ios::in);

    int count = 0; // intialize count to zero

    if(myFile.is_open()) // check if the file is openned
    {
        string line;
        while(getline(myFile, line)) // reading till the file line by line
        {
            count ++;
        }
        myFile.close(); // close the txt file
        
    }
    return count; 
}

string read_specific_line(string fileName, int number_of_lines, int line_to_read)
// this function returns a specific line from a txt file 
{
  // create a variable of type fstream
    fstream myFile;

    // open a file in the read mode 
    myFile.open(fileName,ios::in);

    string line;

    if(myFile.is_open()) // check if the file is openned
    {
        for(int i = 1; i <= line_to_read; i++) // loop untill the file you want to read
        {
          getline(myFile, line);
        }
        myFile.close();
        
    }
    return line; 
}

int requests_per_line(string line)
// This function returns number of requests per line
{
  int number_of_whiteSpaces_per_line = 0;
  
  for(int i = 0; i < line.length() - 1; ++i)
  {
    if (line[i] == ' ')
    {
        number_of_whiteSpaces_per_line++;
    }
  }
  int number_of_request_per_line = number_of_whiteSpaces_per_line + 1;
  return number_of_request_per_line ;
}

int get_floor_number(string line)
// This function takes a request string as in input and returns the floor number
// we will map the floors as b2 b1 G F1 F2 ...... F(n) were b2 is floor zero and F(n) is total floors - 1
{
  if(line[0] == 'G')
  {
      return 2;
  }
  else if (line[0] == 'B')
  {
    return 2 - (int(line[1]) - 48);
  }
  else if (line[0] == 'F')
  {
    return 2 + int(line[1]) - 48;
  }
  else {
    return 0;
  }
}


void create_requests(Client* requests, string request_string, int request_number)
// This function takes a single request sring and create a request object from Client class
{
  int start = 0, direction = 0, end = 0;
  char first_letter = request_string[0];
  // getting object features from a request starts from Basements or Floors
  if( first_letter == 'B' || first_letter == 'F')
  {
    start = get_floor_number({request_string[0], request_string[1], 0x00}); // map starting floor to numeric data
    direction = request_string[3] == 'U' ? 1:0; // map direction to numeric data up to 1 and down to 0
    end = request_string[5] == 'G' ? get_floor_number({request_string[5], 0x00}) : get_floor_number({request_string[5],request_string[6] ,0x00}); // map ending floor to numeric data
  }
  // getting object features from a request starts from Ground
  else
  {
    start = get_floor_number({request_string[0], 0x00}); // map starting floor to numeric data
    direction = request_string[2] == 'U' ? 1:0; // map direction to numeric data up to 1 and down to 0
    end = request_string[4] == 'G' ? get_floor_number({request_string[5], 0x00}) : get_floor_number({request_string[4],request_string[5] ,0x00}); // map starting floor to numeric data
  }
  // intialize a request object from Clients class
  Client request;

  // set member values to the object
  request.setStart(start);
  request.setDir(direction);
  request.setEnd(end);

  requests[request_number] = request;
}

Client* line_to_request(string line)
// This function splits a line and return an array of objects from Client class
{
  int number_of_requests = requests_per_line(line);
  char request_string[16];
  int line_character_index = 0, request_string_character_index = 0;

  // intialize array of objects requests of type Client
  Client* requests = new Client[number_of_requests];
  int request_number = 0;

  while (line[line_character_index] != '.') // loop till reach end of line
  {
    if(line[line_character_index] != ' ') // loop till reach end of a request
    {
      request_string[request_string_character_index] = line[line_character_index];
      request_string_character_index++;
      line_character_index++;
    }
    else
    {
      request_string[request_string_character_index] = 0x00; // end the request string
      line_character_index++;

      // if you reach the end of a request string call create_request function
      create_requests(requests, line, request_number);

      // start  new index for new request
      request_string_character_index = 0;

      // increment request_number
      request_number ++;
    }
  }
  return requests;
}

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