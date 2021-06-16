#include <iostream>
#include <fstream>
#include <string>
#include "Client.h"
#include "Elevator.h"
#include <tuple>
#include <cmath>
bool EXIT = false;
int number_of_elvators = 5;
using namespace std;

string *readLines(string fileName)
// this funtion returns the lines in the txt file
{
  // create a variable of type fstream
  fstream myFile;

  // open a file in the read mode
  myFile.open(fileName, ios::in);

  int count = 0; // intialize count to zero
  string *line = new string[6];
  if (myFile.is_open()) // check if the file is openned
  {
    while (getline(myFile, line[count])) // reading till the file line by line
    {
      count++;
    }
    myFile.close(); // close the txt file
  }
  return line;
}

// string read_specific_line(string fileName, int number_of_lines, int line_to_read)
// // this function returns a specific line from a txt file
// {
//   // create a variable of type fstream
//   fstream myFile;

//   // open a file in the read mode
//   myFile.open(fileName, ios::in);

//   string line;

//   if (myFile.is_open()) // check if the file is openned
//   {
//     getline(myFile, line);

//     myFile.close();
//   }
//   return line;
// }

int requests_per_line(string line)
// This function returns number of requests per line
{
  int number_of_whiteSpaces_per_line = 0;

  for (int i = 0; i < line.length() - 1; ++i)
  {
    if (line[i] == ' ')
    {
      number_of_whiteSpaces_per_line++;
    }
  }
  int number_of_request_per_line = number_of_whiteSpaces_per_line + 1;
  return number_of_request_per_line;
}

int get_floor_number(string line)
// This function takes a request string as in input and returns the floor number
// we will map the floors as b2 b1 G F1 F2 ...... F(n) were b2 is floor zero and F(n) is total floors - 1
{
  if (line[0] == 'G')
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
  else
  {
    return 0;
  }
}

void create_requests(Client *requests, string request_string, int request_number)
// This function takes a single request sring and create a request object from Client class
{
  int start = 0, direction = 0, end = 0;
  char first_letter = request_string[0];
  // getting object features from a request starts from Basements or Floors
  if (first_letter == 'B' || first_letter == 'F')
  {
    start = get_floor_number({request_string[0], request_string[1], 0x00});                                                                        // map starting floor to numeric data
    direction = request_string[3] == 'U' ? 1 : 0;                                                                                                  // map direction to numeric data up to 1 and down to 0
    end = request_string[5] == 'G' ? get_floor_number({request_string[5], 0x00}) : get_floor_number({request_string[5], request_string[6], 0x00}); // map ending floor to numeric data
  }
  // getting object features from a request starts from Ground
  else
  {
    start = get_floor_number({request_string[0], 0x00});                                                                                           // map starting floor to numeric data
    direction = request_string[2] == 'U' ? 1 : 0;                                                                                                  // map direction to numeric data up to 1 and down to 0
    end = request_string[4] == 'G' ? get_floor_number({request_string[5], 0x00}) : get_floor_number({request_string[4], request_string[5], 0x00}); // map starting floor to numeric data
  }
  // intialize a request object from Clients class
  Client request;

  // set member values to the object
  request.setStart(start);
  request.setDir(direction);
  request.setEnd(end);

  requests[request_number] = request;
}

Client *line_to_request(string line)
// This function splits a line and return an array of objects from Client class
{
  int number_of_requests = requests_per_line(line);
  char request_string[16];
  int line_character_index = 0, request_string_character_index = 0;

  // intialize array of objects requests of type Client
  Client *requests = new Client[number_of_requests];
  int request_number = 0;

  do
  {
    if (line[line_character_index] != ' ') // loop till reach end of a request
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
      request_number++;
    }
  } while (line[line_character_index] != 0x00);
  return requests;
}

// this function creats the binary array for declaring the floor and status of each elevator to pass this array to graphic function to draw them
int **GFXTwoD(Elevator **elevator)
{

  int **GFX;
  int columns = number_of_elvators, rows = 10;

  GFX = new int *[rows];
  for (int i = 0; i < rows; i++)
  {
    GFX[i] = new int[columns];
  }
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns + 1; j++)
    {
      if (j == 0)
      {
        GFX[i][j] = 0;
      }
      else if ((rows - i - 1) == elevator[j - 1]->getFloor())
      {
        GFX[i][j] = elevator[j - 1]->getStatus();
      }
      else
      {
        GFX[i][j] = 0;
      }
    }
  }
  return GFX;
}

// takes data of the location and status of elevators as dynamic 2d array
void graphic(int **GFX, int Columns, int Rows)
{
  for (int i = 0; i < Rows; i++)
  {
    for (int j = 0; j < Columns; j++)
    {
      if (j == 0 && (i < (Rows - 3)))
      {
        cout << 'F' << (Rows - (i + 3)) << ':' << '\t'; //print the number of the floor in the first column as the the number next to F is (columns -(i+3))
      }
      else if (j == 0 && (i == (Rows - 3)))
      {
        cout << "G " << ':' << '\t'; // print G in the first column at the ground floor
      }
      else if (j == 0 && (i > (Rows - 3)))
      {
        cout << 'B' << -(Rows - (i + 3)) << ':' << '\t'; // print the number of basment floors in the first column too
      }

      else
      {
        if (GFX[i][j] == 0)
        {
          cout << "|  " << '\t'; // print '|  ' indicating there is no elevator here
        }
        else if (GFX[i][j] == 1)
        {
          cout << "|ــ" << '\t'; // print '|' indacting there is a closed elevator here
        }
        else if (GFX[i][j] == 2)
        {
          cout << "|00" << '\t'; // print '|00' indicating there is an opened elevator here
        }
        else
        {
          cout << "   " << '\t';
        }
      }
    }
    cout << '\n';
  }
}

// tuple<int, int> minmums(Client** clients)
// {
//   int minStart_index = 0;
//   int minEnd_index = 0;
//   int minStart = 100;
//   int minEnd = 100;
//   for (int j = 0; j < number_of_elvators; j++)
//   {
//     for (int k = 0; k < 6; k++)
//     {
//       if (clients[k]->getStart() < minStart)
//       {
//         minStart = clients[k]->getStart();
//         minStart_index = j;
//       }
//       if (clients[k]->getEnd() < minEnd)
//       {
//         minEnd = clients[k]->getEnd();
//         minEnd_index = j;
//       }
//     }
//   }
//   return {minStart, minEnd};
// }

// tuple<int, int> maxmums(Client** clients)
// {
//   int maxStart_index = 0;
//   int maxEnd_index = 0;
//   int maxStart = -100;
//   int maxEnd = -100;
//   for (int j = 0; j < number_of_elvators; j++)
//   {
//     for (int k = 0; k < 6; k++)
//     {
//       if (clients[k]->getStart() > maxStart)
//       {
//         maxStart = clients[k]->getStart();
//         maxStart_index = j;
//       }
//       if (clients[k]->getEnd() > maxEnd)
//       {
//         maxEnd = clients[k]->getEnd();
//         maxEnd_index = j;
//       }
//     }
//   }
//   return {maxStart, mEnd};
// }

void move(Elevator **elevator) //this function moves the elevators one step up or down or just open the doorif the elevator is going to get client out or in
{
  for (int i = 0; i < number_of_elvators; i++)
  {

    if (elevator[i]->getDir() == 1)
    {
      int minStart_index = 0;
      int minEnd_index = 0;
      int minStart = 100;
      int minEnd = 100;
      for (int j = 0; j < number_of_elvators; j++)
      {
        for (int k = 0; k < 6; k++)
        {
          if (elevator[j]->getClients()[k].getStart() < minStart)
          {
            minStart = elevator[j]->getClients()[k].getStart();
            minStart_index = j;
          }
          if (elevator[j]->getClients()[k].getEnd() < minEnd)
          {
            minEnd = elevator[j]->getClients()[k].getEnd();
            minEnd_index = j;
          }
        }
        if ((elevator[i]->getFloor() < elevator[i]->getClients()[minStart].getStart()) && (elevator[i]->getFloor() < elevator[i]->getClients()[minEnd].getEnd()))
        {
          elevator[i]->setFloor(elevator[i]->getFloor() + 1);
          elevator[i]->setStatus(1);
        }
        else if ((elevator[i]->getFloor() == elevator[i]->getClients()[minStart].getStart()) || (elevator[i]->getFloor() == elevator[i]->getClients()[minEnd].getEnd()))
        {
          elevator[i]->setStatus(2);
        }
      }
    }
    else if (elevator[i]->getDir() == 1)
    {
      int maxStart_index = 0;
      int maxEnd_index = 0;
      int maxStart = 100;
      int maxEnd = 100;
      for (int j = 0; j < number_of_elvators; j++)
      {
        for (int k = 0; k < 6; k++)
        {
          if (elevator[j]->getClients()[k].getStart() > maxStart)
          {
            maxStart = elevator[j]->getClients()[k].getStart();
            maxStart_index = j;
          }
          if (elevator[j]->getClients()[k].getEnd() > maxEnd)
          {
            maxEnd = elevator[j]->getClients()[k].getEnd();
            maxEnd_index = j;
          }
        }
        if ((elevator[i]->getFloor() > elevator[i]->getClients()[maxStart].getStart()) && (elevator[i]->getFloor() > elevator[i]->getClients()[maxEnd].getEnd()))
        {
          elevator[i]->setFloor(elevator[i]->getFloor() - 1);
          elevator[i]->setStatus(1);
        }
        else if ((elevator[i]->getFloor() == elevator[i]->getClients()[maxStart].getStart()) || (elevator[i]->getFloor() == elevator[i]->getClients()[maxEnd].getEnd()))
        {
          elevator[i]->setStatus(2);
        }
      }
    }
  }
}

void first_priority(Elevator** elevator_array, Client* new_requests_array, int new_requests_array_length)
{
    for (int i = 0; i<= new_requests_array_length - 1; i++)
    {
        for (int j = 0; j < number_of_elvators -1 ; j++)
        {    if(new_requests_array[i].getDir()=='U' && elevator_array[j]->getDir()=='U')
            {
                int requester_floor = new_requests_array[i].getStart();
                if(elevator_array[j]->getClients()[0].getStart() > requester_floor || elevator_array[j]->getClients()[0].getEnd() > requester_floor)
                {
                    elevator_array[j]->addClient(new_requests_array[i]);
                }
            }
            else if(new_requests_array[i].getDir()=='D' && elevator_array[j]->getDir()=='D')
            {
                int requester_floor = new_requests_array[i].getStart();
                if(elevator_array[j]->getClients()[0].getStart() < requester_floor || elevator_array[j]->getClients()[0].getEnd() < requester_floor)
                {
                    elevator_array[j]->addClient(new_requests_array[i]);
                }
            }
        }
    }

}


void second_priority(Elevator** elevator_array, Client* new_requests_array, int new_requests_array_length)
{
    for (int i = 0; i<= new_requests_array_length - 1; i++)
    {
        
        for(int k = 0; k<= number_of_elvators - 1; k++)
        {
            if(new_requests_array[i].getDir()=='D' && elevator_array[k]->getDir()=='U')
            {
                int requester_floor = new_requests_array[i].getStart();
                if(elevator_array[k]->getClients()[0].getStart() > requester_floor || elevator_array[k]->getClients()[0].getEnd() > requester_floor)
                {
                    // search for empty elevator and assign it to client
                    for (int j = 0; j < number_of_elvators -1 ; j++)
                    {
                        if(elevator_array[j]->getNum_of_clients() == 0)
                        {
                            elevator_array[j]->addClient(new_requests_array[i]);
                        }
                    }
                    

                }
            }
            else if(new_requests_array[i].getDir()=='U' && elevator_array[k]->getDir()=='D')
            {
                int requester_floor = new_requests_array[i].getStart();
                if(elevator_array[k]->getClients()[0].getStart() < requester_floor || elevator_array[k]->getClients()[0].getEnd() < requester_floor)
                {
                    // search for empty elevator and assign it to client
                    for (int j = 0; j < number_of_elvators -1 ; j++)
                    {
                        if(elevator_array[j]->getNum_of_clients() == 0)
                        {
                            elevator_array[j]->addClient(new_requests_array[i]);
                        }
                    }
                }
            }
        }
    }
}
void nearst_elvator(Elevator** elevator_array, Client* new_requests_array, int new_requests_array_length)
{
    int min_distance = 999, distance, flag;
    for (int i = 0; i<= new_requests_array_length - 1; i++)
    {
        for (int j = 0; j < number_of_elvators - 1; j++)
        {
             distance = abs(new_requests_array[i].getStart() - elevator_array[i]->getFloor());
             if (distance < min_distance)
             {
                 min_distance = distance;
                 flag = j;
             }
        }
        elevator_array[flag]->addClient(new_requests_array[i]);
        
    }
}