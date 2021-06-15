#ifndef Client_H
#define Client_H

#include <iostream>
using namespace std;

class Client
{
private:
    int start, end, status; // if status == 0 so the client is not in the elvator elif status == 1 so in the elvator elif == 2 so arrived to destination
    char dir;
public:
    Client();
    Client(int, int, char, int);
    ~Client();
    void setStart(int);
    void setEnd(int);
    void setDir(char);
    int getStart();
    int getEnd();
    char getDir();
    int getStatus();
    void setStatus(int);

};

Client::Client()
{
    start = 2;
    end = 2;
    dir = 'U';
    status = 0;
}
Client::Client(int s, int e, char d, int st){   // s, e, d, st are dummy variables and theur scope is just in this function 
    start = s;
    end = e;
    dir = d;
    status = st;
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
int Client::getStatus(){
    return status;
}
void Client::setStatus(int st){
    status = st;
}
Client::~Client()
{
}




#endif