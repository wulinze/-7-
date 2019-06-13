//
// Created by 82490 on 2019/5/12.
//

//
// Created by root on 5/13/19.
//

#include <sstream>
#include <unistd.h>
#include "ProcessManager.h"

#define RAM_MAX 50
#define SHARED_MAX 10
#define MUTEX_MAX 10
#define LOWER_BLOCK 3


int ProcessManager::MAX_SIZE = RAM_MAX; //MAX_SIZE of Process
int ProcessManager::SHARED_SIZE = SHARED_MAX;
int ProcessManager::MUTEX_SIZE = MUTEX_MAX;

bool ProcessManager::Block_empty() {
    return this->block_queue.empty();
}
bool ProcessManager::Ready_empty() {
    return this->ready_queue.empty();
}
bool ProcessManager::Ready_Suspend_empty() {
    return this->ready_suspend_queue.empty();
}
bool ProcessManager::Block_Suspend_empty() {
    return this->block_suspend_queue.empty();
}
bool ProcessManager::Is_full(int num, bool exist) {
    if(!exist)
        return num>MAX_SIZE;
    else
        return false;
}
bool ProcessManager::Is_Satisfied(Process &process) {
//    cout << "SHARED:" << ProcessManager::SHARED_SIZE << endl;
//    cout << "MUTEX:" << ProcessManager::MUTEX_SIZE << endl;
    return true;
//    return process.getPCB().Resource_list[Source::Shared] <= ProcessManager::SHARED_SIZE &&
//           process.getPCB().Resource_list[Source::Mutex] <= ProcessManager::MUTEX_SIZE;
}
void ProcessManager::AddBlock(Process *process, bool back , bool exist) {
    if(!Is_full(process->getPCB().size,exist)){
        process->getPCB().setState(State::Block);
        if(back){
            this->block_queue.emplace_back(process);
        } else{
            this->block_queue.insert(block_queue.begin(),1,process);
        }
        if(!exist)
            ProcessManager::MAX_SIZE -= process->getPCB().size;
    } else{
        BlockSuspend(process);
    }
}
void ProcessManager::AddReady(Process *process, bool back , bool exist) {
    if(!Is_full(process->getPCB().size,exist)){
        process->getPCB().setState(State::Ready);
        if(back){
            this->ready_queue.emplace_back(process);
        } else{
            this->ready_queue.insert(ready_queue.begin(),1,process);
        }
        if(!exist)
            ProcessManager::MAX_SIZE -= process->getPCB().size;
    } else{
        if(!Block_empty()){
            int num=0;
            int rstart=block_queue.size()-1;
            while(rstart>=0){
                num += block_queue[rstart]->getPCB().size;
                rstart--;
                if(num >= process->getPCB().size){
                    for(int i=block_queue.size()-1;i>rstart;i--){
                        BlockSuspend(block_queue[i]);
                        ProcessManager::MAX_SIZE += block_queue[i]->getPCB().size;
                        block_queue.erase(block_queue.begin()+i);
                    }
                    this->AddReady(process,true,false);
                    // ProcessManager::MAX_SIZE -= process->getPCB().size;
                    return;
                }
            }
            ReadySuspend(process);
        } else{
            ReadySuspend(process);
        }
    }
}
Process * ProcessManager::getReady() {
    if(!Ready_empty()){
        Process* p =*this->ready_queue.begin();
        this->ready_queue.erase(ready_queue.begin());
        if(!Ready_Suspend_empty() && !Is_full(ready_suspend_queue[0]->getPCB().size))
            ReadyActive();
        return p;
    }else{
        if(!Ready_Suspend_empty() && !Is_full(ready_suspend_queue[0]->getPCB().size)){
            ReadyActive();
            Dispatch();
        }
        buffer << "Ready Queue is Empty" << endl;
        return nullptr;
    }
}
Process * ProcessManager::getBlock() {
    if(!Block_empty()){
        Process* p =*this->block_queue.begin();
        this->block_queue.erase(block_queue.begin());
        if(!Block_Suspend_empty() && !Is_full(block_suspend_queue[0]->getPCB().size))
            BlockActive();
        return p;
    }else{
        if(!Block_Suspend_empty() && !Is_full(block_suspend_queue[0]->getPCB().size)){
            BlockActive();
            return *this->block_queue.begin();
        }
        buffer << "Block Queue is Empty" << endl;
        return nullptr;
    }
}
Process * ProcessManager::getRun() {
    return this->runing;
}
Process * ProcessManager::getReadySuspend() {
    if(!Ready_Suspend_empty()){
        Process* p =*this->ready_suspend_queue.begin();
        this->ready_suspend_queue.erase(ready_suspend_queue.begin());
        return p;
    }else{
        buffer << "Ready Suspend Queue is Empty" << endl;
        return nullptr;
    }
}
Process * ProcessManager::getBlockSuspend() {
    if(!Block_Suspend_empty()){
        Process* p =*this->block_suspend_queue.begin();
        this->block_suspend_queue.erase(block_suspend_queue.begin());
        return p;
    }else{
        buffer << "Block Suspend Queue is Empty" << endl;
        return nullptr;
    }
}

void ProcessManager::Action(int opt) {
    switch (opt){
        case 0: {
            this->Create();
//            cout << "Success Create" << endl;
            break;
        }
        case 1: {
            this->Dispatch();
//            cout << "Success Dispatch" << endl;
            break;
        }
        case 2: {
            this->EventOccur();
//            cout << "Success EventOccur" << endl;
            break;
        }
        case 3: {
            this->EventWait();
//            cout << "Success EventWait" << endl;
            break;
        }
        case 4: {
            this->TimeOut();
//            cout << "Success TimeOut" << endl;
            break;
        }
        case 5: {
            this->Exit();
//            cout << "Success Exit" << endl;
            break;
        }
        default:{
            buffer <<  "There isn't a Action Option" << endl;
        }
    }
}
void ProcessManager::Show() {
    system("clear");
     cout << "Ready:";
     for(auto & i : ready_queue){
         cout << *i << " ";
     }
     cout << endl;
     cout << "Block:";
     for(auto & i : block_queue){
         cout << *i << " ";
     }
     cout << endl;
     if (this->runing == nullptr) {
         cout << "Running:" << "" << endl;
     } else {
         cout << "Running:" << *(this->runing) << endl;
     }
     cout << "Block Suspend:";
     for(auto & i : block_suspend_queue){
         cout << *i << " ";
     }
     cout << endl << "Ready Suspend:";
     for(auto & i : ready_suspend_queue){
         cout << *i << " ";
     }
     cout << endl;
//    cout << "left Resource SHARED:" << ProcessManager::SHARED_SIZE << "/" << SHARED_MAX <<endl;
//    cout << "Left Resource MUTEX:" << ProcessManager::MUTEX_SIZE << "/" << MUTEX_MAX << endl;
    cout << "Left SIZE:" << ProcessManager::MAX_SIZE << "/" << RAM_MAX << endl;
    cout << buffer.str() << endl;
    cout << "------------------------------------------" <<endl;
    for(auto &i:this->P_List){
        cout << *i << endl;
    }
     cout << "//Create:0 " << "Dispatch:1 " << "EventOccur:2 "
          << "EventWait:3 " << "TimeOut:4 " << "Exit:5" << endl;
     cout << "input:" ;
    buffer.str("");
}

void ProcessManager::Create(std::string val) {
    Process* p = new Process(std::move(val));
//    cin >> *p;
    if(Is_Satisfied(*p)){
        this->AddReady(p);
//        ProcessManager::SHARED_SIZE -= p->getPCB().Resource_list[Source::Shared];
//        ProcessManager::MUTEX_SIZE -= p->getPCB().Resource_list[Source::Mutex];
//        cout << "MAX:" << ProcessManager::MAX_SIZE << endl;
    } else{
        this->AddBlock(p, false);
    }
    this->P_List.emplace_back(p);
    if(!this->runing)Dispatch();
    buffer << "Success Create" << endl;
}
void ProcessManager::Exit() {
    if(this->runing != nullptr){
        this->runing->getPCB().setState(State::Exit);
//        ProcessManager::SHARED_SIZE += this->runing->getPCB().Resource_list[Source::Shared];
//        ProcessManager::MUTEX_SIZE += this->runing->getPCB().Resource_list[Source::Mutex];
        ProcessManager::MAX_SIZE += this->runing->getPCB().size;
//        delete(this->runing);
        this->runing= nullptr;
        Dispatch();
        buffer << "Success Exit" << endl;
    }else{
        buffer << "Fail to Exit" << endl;
    }
}
//Three State Transfer
void ProcessManager::TimeOut() {
    if(this->runing){
        Process* p = this->getRun();
        this->runing = nullptr;
        Dispatch();
        AddReady(p, true , true);
        buffer << "Success TimeOut" << endl;
    } else{
        buffer << "Fail TimeOut" << endl;
    }
}
void ProcessManager::EventOccur() {
    Process* p = getBlock();
    if(p){
        AddReady(p, false, true);
        if(!this->runing)Dispatch();
        buffer << "Success EventOccur" << endl;
    } else{
        buffer << "Fail EventOccur" << endl;
    }
}
void ProcessManager::EventWait() {
    if(this->runing){
        AddBlock(this->getRun(), false, true);
        this->runing = nullptr;
        Dispatch();
        buffer << "Success EventWait" << endl;
    }else{
        buffer << "Fail EventWait" << endl;
    }
}
void ProcessManager::Dispatch() {
    Process* p = (getReady());
    if(p){
        if(this->runing != nullptr){
            this->runing->getPCB().setState(State::Ready);
            AddReady(this->runing,false,true);
//            ProcessManager::SHARED_SIZE += this->runing->getPCB().Resource_list[Source::Shared];
//            ProcessManager::MUTEX_SIZE += this->runing->getPCB().Resource_list[Source::Mutex];
            // ProcessManager::MAX_SIZE += this->runing->getPCB().size;
//            delete this->runing;
        }
        this->runing = p;
        this->runing->getPCB().setState(State::Run);
        buffer << "Success Dispatch" << endl;
    }else{
        buffer << "Fail Dispatch" << endl;
    }
}
//Seven State Transfer
void ProcessManager::BlockActive() {
    AddBlock(getBlockSuspend(), true , false);
}
void ProcessManager::BlockSuspend(Process* p) {
    p->getPCB().state = State ::BlockSuspend;
    block_suspend_queue.emplace_back(p);
}
void ProcessManager::ReadyActive() {
    AddReady(getReadySuspend(), true , false);
}
void ProcessManager::ReadySuspend(Process* p) {
    p->getPCB().state = State ::ReadySuspend;
    ready_suspend_queue.emplace_back(p);
}

ProcessManager::ProcessManager() {
    this->runing = nullptr;
    stringstream ss;
    for (int i = 0; i < 3; ++i) {
        ss.str("");
        ss << i;
        this->Create("Process:" + ss.str());
    }
}
ProcessManager::~ProcessManager() {
    cout << "Manager End" << endl;
}






