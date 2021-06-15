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
    int status; 
    Client* clients; //array of clients existing in the elevator 
     
public:
    Elevator();
    ~Elevator();
    void setFloor(int);
    void setDir(char);
    void setStatus(int);
    
    
    int getFloor();
    char getDir();
    int getStatus();
    void addClient(const Client&, int);
    Client* getClients();
};

//initiate closed elevators in the ground floor
Elevator::Elevator()
{
    floor = 2;
    dir = 'E';
    status = 1;
    clients = new Client[1];
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
// adds a new client object to the array of exiting clients i the elevator
void Elevator::addClient(const Client& R, int size){
    Client *temp = new Client[size + 1];
    for (int i = 0; i < size; i++)
    {
        temp[i] = clients[i];
    }
    delete[] clients;
    temp[size] = R;
    this->clients = temp;
    // clients = new Client[size + 1];
    // for (int i = 0; i < (size + 1); i++)
    // {
    //     clients[i] = temp[i];
    // }
    // delete[] temp;
}
//returns the array of clients existing in the elevator
Client* Elevator::getClients(){
    return clients;
}

Elevator::~Elevator()
{
}
#endif