#ifndef Client_H
#define Client_H

#include <iostream>
using namespace std;

class Client
{
private:
    int start, end;
    char dir;
public:
    Client();
    Client(int, int, char);
    ~Client();
    void setStart(int);
    void setEnd(int);
    void setDir(char);
    int getStart();
    int getEnd();
    char getDir();

};

Client::Client()
{
    start = 2;
    end = 2;
    dir = 'U';
}
Client::Client(int s, int e, char d){
    start = s;
    end = e;
    dir = d;
}
// set member start 
void Client::setStart(int s){
    start = s;
}
void Client::setEnd(int e){
    end =e;
}
void Client::setDir(char d){
    dir = d;
}
int Client::getStart(){
    return start;
}
int Client::getEnd(){
    return end;
}
char Client::getDir(){
    return dir;
}
Client::~Client()
{
}




#endif