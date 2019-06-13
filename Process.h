//
// Created by root on 5/13/19.
//

#ifndef PROCESS_PROCESS_H
#define PROCESS_PROCESS_H


#include <string>
#include <iostream>
#include <map>

enum State{
    New=0,Ready=1,Run=2,Block=3,Exit=4,ReadySuspend=5,BlockSuspend=6
};
enum Source{
    Shared=0,Mutex=1
};
struct PCB{

    int pid;
    State state;
//    std::map<Source ,int> Resource_list;
    int size;


    void setPid(int pid);
    void setState(State state);
    void setState(int state);

    int getPid();
    State getState();
};
class Process {
private:
    static int process_num;
    PCB PCB_obj;
    std::string val;
public:
    explicit Process(std::string val="new process");

    std::string getVal();
    void setVal(std::string);

    PCB& getPCB();

    friend std::ostream& operator<<(std::ostream& out,Process& p);
    friend std::istream& operator>>(std::istream& in,Process& p);

    virtual ~Process();
};


#endif //PROCESS_PROCESS_H
