#include <iostream>
#include "Logger.h"
#include "thread"
using namespace  std;
void test(){
    for(int i=0;i<80000;i++) {
        writelog(LogLevel::FATAL,__FILE__,__LINE__,"fdafa");
    }
    while (1){}
}

int main() {



    thread th1(test);
    th1.join();


    return 0;
}
