#include "Process.h"
#include <ctime>
#include <string>
#include <stdlib.h>
#include <ctime>

using namespace std;


int Process::process_num=0;

Process::Process(std::string val) {
    Process::process_num++;
    std::cout << "Creating a Process:" << Process::process_num << endl;
    this->PCB_obj.pid = Process::process_num;
    this->PCB_obj.state = State::New;
    srand(time(0));
    this->PCB_obj.size = (rand() % 10)+ 1;
//    cout << "size=" <<this->PCB_obj.size << endl;
    this->val = std::move(val);
}
void Process::setVal(std::string val){
    this->val = std::move(val);
}
std::string Process::getVal(){
    return this->val;
}
PCB& Process::getPCB(){
    return this->PCB_obj;
}
Process::~Process() {
    std::cout << "Exit a Process" << endl;
}

std::ostream &operator<<(std::ostream& out, State &s){
    switch (s)
    {
    case State::Ready:
        cout << "Ready";
        break;
    case State::Block:
        cout << "Block";
        break;
    case State::New:
        cout << "New";
        break;
    case State::Exit:
        cout << "Exit";
        break;
    case State::BlockSuspend:
        cout << "BlockSuspend";
        break;
    case State::ReadySuspend:
        cout << "ReadySuspend";
        break;
    case State::Run:
        cout << "Running";
        break;
    default:
        break;
    }
    return out;
}
std::ostream &operator<<(std::ostream& out, Process &p) {
    out << "(pid:" << p.getPCB().pid << "\tval:" << p.getVal() << "\tsize:" << p.getPCB().size 
        << "\tstate:" << p.getPCB().state << ')';
//        << ",shared_size:" << p.getPCB().Resource_list[Source::Shared]
//        << ",mutex_size:" << p.getPCB().Resource_list[Source::Mutex] <<')';
    return out;
}
std::istream &operator>>(std::istream &in, Process &p) {
    string val;
    int num;
//    p.getPCB().size = 0;
    cout << "input val:" ;
    in >> val;
    p.setVal(val);
//    cout << "Resource list:" << endl << "Shared:";
//    in >> num;
//    p.getPCB().Resource_list[Source::Shared] = num;
////    p.getPCB().size += num;
//    cout << "Mutex:";
//    in >> num;
//    p.getPCB().Resource_list[Source::Mutex] = num;
//    p.getPCB().size += num;
    return in;
}


void PCB::setPid(int pid){
    this->pid = pid;
}
State PCB::getState(){
    return this->state;
}
void PCB::setState(State state) {
    this->state = state;
}
void PCB::setState(int state) {
    this->state = State(                                                                                                                                                                                                                                                                                                                                                                                                                                      state);
}
int PCB::getPid(){
    return this->pid;
}
