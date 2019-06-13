#include <iostream>
#include "Process.h"
#include "ProcessManager.h"

int main() {
    int opt=0;
    auto *pm = new ProcessManager();
    pm->Show();
    while(cin >> opt){
        pm->Action(opt);
        pm->Show();
    }
    return 0;
}