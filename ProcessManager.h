//
// Created by 82490 on 2019/5/12.
//

#ifndef PROCESS_PROCESSMANAGER_H
#define PROCESS_PROCESSMANAGER_H


#include "Process.h"
#include <vector>
#include <sstream>

using namespace std;

class ProcessManager {
private:
//	vector<Process&> queue_progress;
    std::vector<Process*> ready_queue,block_queue;  //queue
    std::vector<Process*> ready_suspend_queue,block_suspend_queue;
    Process* runing;

    std::vector<Process*> P_List;

    std::stringstream buffer;   //Operate BUffer

    static int MAX_SIZE,SHARED_SIZE,MUTEX_SIZE;

    bool Ready_empty();
    bool Block_empty();
    bool Ready_Suspend_empty();
    bool Block_Suspend_empty();

    static bool Is_full(int num , bool exist= false);
    static bool Is_Satisfied(Process& process);
public:
    ProcessManager();

    Process * getReady();
    Process * getRun();
    Process * getBlock();
    Process * getReadySuspend();
    Process * getBlockSuspend();

//	bool ManageProcess();
    void Show();
    void Action(int opt);

    void AddReady(Process* process, bool back= true, bool exist= false);
    void AddBlock(Process* process, bool back= true, bool exist= false);
    void Dispatch();
    void TimeOut();
    void EventOccur();
    void EventWait();
    void Create(std::string val="new process");
    void Exit();

    void ReadySuspend(Process* p);
    void BlockSuspend(Process* p);
    void ReadyActive();
    void BlockActive();

    virtual ~ProcessManager();
};


#endif //PROCESS_PROCESSMANAGER_H
