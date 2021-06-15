#ifndef Elevator_E
#define Elevator_E
#include <iostream>
#include "Client.h"
using namespace std;


class Elevator
{
private:
    int floor;
    char dir;
    int status, num_of_clients; 
    Client clients[6]; //array of clients existing in the elevator where maximum clients per elvator is 6
     
public:
    Elevator();
    ~Elevator();
    void setFloor(int);
    void setDir(char);
    void setStatus(int);
    
    
    int getFloor();
    char getDir();
    int getStatus();
    int getNum_of_clients();
    void addClient(Client&);
    Client* getClients();
};

//initiate closed elevators in the ground floor
Elevator::Elevator()
{
    floor = 2;
    dir = 'E';
    status = 1;
    num_of_clients = 0;
}
void Elevator::setFloor(int f){
    floor = f;
}
void Elevator::setDir(char d){
    dir = d;
}
void Elevator::setStatus(int s){
    status = s;
}

int Elevator::getFloor(){
    return floor;
}
int Elevator::getStatus(){
    return status;
}
char Elevator::getDir(){
    return dir;
}
int Elevator::getNum_of_clients(){
    return num_of_clients;
}
// adds a new client object to the array of exiting clients i the elevator
void Elevator::addClient(Client& R){

    // Client c1();
    // c1.setStart(R.getStart());
    // for(int i = 0; i < 6; i++)
    // {
    //     clients[i] = Client();
    // }
    if (num_of_clients <= 5)
    {
        clients[num_of_clients] = R;
        num_of_clients++;
    }
    else if (num_of_clients == 5)
    {
        for (int i = 0; i < 6; i++)
        {
            if (clients[i].getStatus() == 2)
            {
                clients[i] = R;
                break;
            }
        }
        
    }
   
}
//returns the array of clients existing in the elevator
Client* Elevator::getClients(){
    return clients;

}

Elevator::~Elevator()
{
}
#endif