#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <thread>
#include <thread>
#include <mutex>
#include <unistd.h>


using namespace std;
struct section {
    int type,processID,priority,burstTime,remainTime;
};
mutex m;
static vector<section> data0,data1;
static int times[100],processNum,time_quantum0,time_quantum1,memType,logType;
static vector<int> memAns,logAns;
static string address = "/Users/evil_favoured/Desktop/test.txt";
static int time0,time1;
static section newSection;

void deleteFromData(){
    int proID;
    for(int i=0;i<data0.size();i++){
        if(data0[i].priority<=0){
            proID = data0[i].processID;
            data0.erase(data0.begin() + i);
            i = 0;
            for(int j=0;j<data0.size();j++){
                if(data0[j].processID==proID){
                    data0[j].priority--;
                }
            }
            for(int j=0;j<data1.size();j++){
                if(data1[j].processID==proID){
                    data1[j].priority--;
                }
            }
        }
    }
    for(int i=0;i<data1.size();i++){
        if(data1[i].priority<=0){
            proID = data1[i].processID;
            data1.erase(data1.begin() + i);
            i = 0;
            for(int j=0;j<data0.size();j++){
                if(data0[j].processID==proID){
                    data0[j].priority--;
                }
            }
            for(int j=0;j<data1.size();j++){
                if(data1[j].processID==proID){
                    data1[j].priority--;
                }
            }
        }
    }

}

int getNextProcess(int time,int type){
    if(type == 0){
        for(int i=0;i<data0.size();i++){
            int t = times[data0[i].processID];
            //printf("memory :%d  %d\n",t,i);
            if(t <= time&&data0[i].priority==1){
                return i;
            }
            
        }
        return -1;
    }
    else{
        for(int i=0;i<data1.size();i++){
            int t = times[data1[i].processID];
            //printf("logic : %d  %d\n",t,i);
            if(t <= time&&data1[i].priority==1){
                return i;
            }
            
        }
        return -1;
    }
}


void sortRR(int type){
    if(type == 0){
        sort( data0.begin(), data0.end(),
             []( const section &left, const section &right )
             { if(left.priority == right.priority)
                 return left.processID < right.processID;
             else
                 return ( left.priority < right.priority );});
    }
    else{
        sort( data1.begin(), data1.end(),
             []( const section &left, const section &right )
             {if(left.priority == right.priority)
                 return left.processID < right.processID;
             else
                 return ( left.priority < right.priority );});
    }
}

void sortSRT(int type){
    if(type == 0){
    sort( data0.begin(), data0.end(),
         []( const section &left, const section &right )
         { if(left.remainTime == right.remainTime)
             return ( left.processID < right.processID );
         else
             return (left.remainTime < right.remainTime);} );
    }
    else{
        sort( data1.begin(), data1.end(),
             []( const section &left, const section &right )
             { if(left.remainTime == right.remainTime)
                 return ( left.processID < right.processID );
             else
                 return (left.remainTime < right.remainTime);} );
    }
}

void sortHRRN(int type){
    if(type == 0){
        sort( data0.begin(), data0.end(),
             []( const section &left, const section &right )
             { if(time0 >= times[left.processID] && time0 >= times[right.processID])
                 return ((time0 - times[left.processID])/left.burstTime) > ((time0 - times[right.processID])/right.burstTime);
             else if(time0 >= times[left.processID] && time0 < times[right.processID])
                 return true;
             else
                 return false;} );
    }
    else{
        sort( data1.begin(), data1.end(),
             []( const section &left, const section &right )
             { if(time1 >= times[left.processID] && time1 >= times[right.processID])
                 return ((time1 - times[left.processID])/left.burstTime) > ((time1 - times[right.processID])/right.burstTime);
             else if(time1 >= times[left.processID] && time1 < times[right.processID])
                 return true;
             else
                 return false;} );
    }
}


void computeSRT(int type){
    int count;
    if(type==0){
        if(getNextProcess(time0,0) >=0){
            count=getNextProcess(time0,0);
            newSection = data0[count];
        
            if(newSection.remainTime <= time_quantum0&&newSection.priority==1){
                time0+=newSection.remainTime;
                printf("kamtar memrory:P[%d] time0:%d  priority:%d\n",newSection.processID,time0,newSection.priority);
                for(int i=0;i<newSection.remainTime;i++)
                    memAns.push_back(newSection.processID);
                data0[count].priority = 0;
            }
            else if(newSection.priority==1){
                time0+=time_quantum0;
                printf("bishtar memrory:P[%d] time0:%d  priority:%d\n",newSection.processID,time0,newSection.priority);
                for(int i=0;i<time_quantum0;i++)
                    memAns.push_back(newSection.processID);
                int remainTime=newSection.remainTime-time_quantum0;
                newSection.remainTime = remainTime;
                data0.erase(data0.begin()+count);
                data0.push_back(newSection);
                
            }
        }
        else{
            memAns.push_back(0);
            time0++;
            printf("memrory:P[%d] time0:%d\n",0,time0);
        }
    }
    else{
        if(getNextProcess(time1,1) >=0){
            count=getNextProcess(time1,1);
            newSection = data1[count];
            
            if(newSection.remainTime <= time_quantum1&&newSection.priority==1){
                time1+=newSection.remainTime;
                printf("kamatar logic:P[%d] time1:%d  priority:%d\n",newSection.processID,time1,newSection.priority);
                for(int i=0;i<newSection.remainTime;i++)
                    logAns.push_back(newSection.processID);
                data1[count].priority = 0;
            }
            else if(newSection.priority==1){
                time1+=time_quantum1;
                printf("bishtar logic:P[%d] time1:%d  priority:%d\n",newSection.processID,time1,newSection.priority);
                for(int i=0;i<time_quantum1;i++)
                    logAns.push_back(newSection.processID);
                int remainTime=newSection.remainTime-time_quantum1;
                newSection.remainTime = remainTime;
                data1.erase(data1.begin()+count);
                data1.push_back(newSection);
                
            }
        }
        else{
            logAns.push_back(0);
            time1++;
            printf("logic:P[%d] time1:%d\n",0,time1);
        }
    }
}

void computeRR(int type){
    int count;
    if(type==0){
        if(getNextProcess(time0,0) >=0){
            count=getNextProcess(time0,0);
            newSection = data0[count];
            
            if(newSection.remainTime <= time_quantum0&&newSection.priority==1){
                time0+=newSection.remainTime;
                printf("kamtar memrory:P[%d] time0:%d  priority:%d\n",newSection.processID,time0,newSection.priority);
                for(int i=0;i<newSection.remainTime;i++)
                    memAns.push_back(newSection.processID);
                data0[count].priority = 0;

            }
            else if(newSection.priority==1){
                time0+=time_quantum0;
                printf("bishtar memrory:P[%d] time0:%d  priority:%d\n",newSection.processID,time0,newSection.priority);
                for(int i=0;i<time_quantum0;i++)
                    memAns.push_back(newSection.processID);
                int remainTime=newSection.remainTime-time_quantum0;
                newSection.remainTime = remainTime;
                data0.erase(data0.begin()+count);
                data0.push_back(newSection);
                
            }
        }
        else{
            memAns.push_back(0);
            time0++;
            printf("memrory:P[%d] time0:%d\n",0,time0);
        }
    }
    else{
        if(getNextProcess(time1,1) >=0){
            count=getNextProcess(time1,1);
            newSection = data1[count];
                
            if(newSection.remainTime <= time_quantum1&&newSection.priority==1){
                time1+=newSection.remainTime;
                printf("kamatar logic:P[%d] time1:%d  priority:%d\n",newSection.processID,time1,newSection.priority);
                for(int i=0;i<newSection.remainTime;i++)
                    logAns.push_back(newSection.processID);
                data1[count].priority = 0;
            }
            else if(newSection.priority==1){
                time1+=time_quantum1;
                printf("bishtar logic:P[%d] time1:%d  priority:%d\n",newSection.processID,time1,newSection.priority);
                for(int i=0;i<time_quantum1;i++)
                    logAns.push_back(newSection.processID);
                int remainTime=newSection.remainTime-time_quantum1;
                newSection.remainTime = remainTime;
                data1.erase(data1.begin()+count);
                data1.push_back(newSection);
            }
        }
        else{
            logAns.push_back(0);
            time1++;
            printf("logic:P[%d] time1:%d\n",0,time1);
        }
    }
}

void computeHRRN(int type){
    int count;
    if(type==0){
        if(getNextProcess(time0,0) >=0){
            count=getNextProcess(time0,0);
            newSection = data0[count];
            if(newSection.priority==1){
                time0+=newSection.remainTime;
                printf("kamtar memrory:P[%d] time0:%d  priority:%d\n",newSection.processID,time0,newSection.priority);
                for(int i=0;i<newSection.remainTime;i++)
                    memAns.push_back(newSection.processID);
                data0[count].priority = 0;
                
            }
        }
        else{
            memAns.push_back(0);
            time0++;
            printf("memrory:P[%d] time0:%d\n",0,time0);
        }
    }
    else{
        if(getNextProcess(time1,1) >=0){
            count=getNextProcess(time1,1);
            newSection = data1[count];
            if(newSection.priority==1){
                time1+=newSection.remainTime;
                printf("kamtar logic:P[%d] time1:%d  priority:%d\n",newSection.processID,time1,newSection.priority);
                for(int i=0;i<newSection.remainTime;i++)
                    logAns.push_back(newSection.processID);
                data1[count].priority = 0;
                
            }
        }
        else{
            logAns.push_back(0);
            time1++;
            printf("logic:P[%d] time1:%d\n",0,time1);
        }
    }
}


void readFile(){
    section temp;
    string str;
    string sentence[100];
    ifstream myfile (address);
    getline (myfile,str);
    if(str.at(8)=='R'){
        memType = 1;
        time_quantum0 = str.at(11) -'0';
    }
    else if(str.at(8)=='S'){
        memType = 2;
        time_quantum0 = str.at(12) -'0';
    }
    else if(str.at(8)=='H'){
        memType = 3;
    }
    else if(str.at(8)=='M'){
        memType = 4;
    }
    
    getline (myfile,str);
    if(str.at(7)=='R'){
        logType = 1;
        time_quantum1 = str.at(10) -'0';
    }
    else if(str.at(7)=='S'){
        logType = 2;
        time_quantum1 = str.at(11) -'0';
    }
    else if(str.at(7)=='H'){
        logType = 3;
    }
    else if(str.at(7)=='M'){
        logType = 4;
    }
    getline (myfile,str);
    processNum = atoi(str.c_str());
    int p =0;
    while(getline (myfile,sentence[p])&&p<processNum)
    {
        p++;
    }
    myfile.close();
    for(int j=0;j<processNum;j++){
        istringstream iss(sentence[j]);
        vector<string> tokens{istream_iterator<string>{iss},
            istream_iterator<string>{}};
        times[j+1] = atoi(tokens[1].c_str());
        for(int i=2;i<tokens.size();i++){
            if(tokens[i].at(0)=='M'){
                temp.type = 0;
                temp.processID = j+1;
                temp.priority = i-1;
                temp.burstTime = tokens[i].at(1)- '0';
                temp.remainTime = temp.burstTime;
                data0.push_back(temp);
            }
            else if(tokens[i].at(0)=='L'){
                temp.type = 1;
                temp.processID = j+1;
                temp.priority = i-1;
                temp.burstTime = tokens[i].at(1)- '0';
                temp.remainTime = temp.burstTime;
                data1.push_back(temp);
            }
        }
        tokens.clear();
    }

}

void schedule(int mem,int log){
    switch (mem) {
        case 1:
            switch (log) {
                case 1:
                    m.lock();
                    deleteFromData();
                    computeRR(0);
                    computeRR(1);
                    m.unlock();
                    break;
                case 2:
                    m.lock();
                    deleteFromData();
                    computeRR(0);
                    computeSRT(1);
                    sortSRT(1);
                    m.unlock();
                    break;
                case 3:
                    m.lock();
                    deleteFromData();
                    computeRR(0);
                    computeHRRN(1);
                    sortHRRN(1);
                    m.unlock();
                    break;
                default:
                    break;
            }
            break;
        case 2:
            switch (log) {
                case 1:
                    m.lock();
                    deleteFromData();
                    computeSRT(0);
                    computeRR(1);
                    sortSRT(0);
                    m.unlock();
                    break;
                case 2:
                    m.lock();
                    deleteFromData();
                    computeSRT(0);
                    computeSRT(1);
                    sortSRT(0);
                    sortSRT(1);
                    m.unlock();
                    break;
                case 3:
                    m.lock();
                    deleteFromData();
                    computeSRT(0);
                    computeHRRN(1);
                    sortSRT(0);
                    sortHRRN(1);
                    m.unlock();
                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch (log) {
                case 1:
                    m.lock();
                    deleteFromData();
                    computeHRRN(0);
                    computeRR(1);
                    sortHRRN(0);
                    m.unlock();
                    break;
                case 2:
                    m.lock();
                    deleteFromData();
                    computeHRRN(0);
                    computeSRT(1);
                    sortHRRN(0);
                    sortSRT(1);
                    m.unlock();
                    break;
                case 3:
                    m.lock();
                    deleteFromData();
                    computeHRRN(0);
                    computeHRRN(1);
                    sortHRRN(0);
                    sortHRRN(1);
                    m.unlock();
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

int main() {
    time0=0,time1=0;
    readFile();
    //memType=1;
    //logType=3;
    vector<thread> threads;
    switch (memType) {
        case 1:
            switch (logType) {
                case 1:
                    sortRR(0);
                    sortRR(1);
                    while(!data0.empty()||!data1.empty()){
                        threads.push_back(thread(schedule,1,1));
                    }
                    break;
                case 2:
                    sortRR(0);
                    sortSRT(1);
                    while(!data0.empty()||!data1.empty()){
                        threads.push_back(thread(schedule,1,2));
                    }
                    break;
                case 3:
                    sortRR(0);
                    sortRR(1);
                    while(!data0.empty()||!data1.empty()){
                        threads.push_back(thread(schedule,1,3));
                    }
                    break;
                default:
                    break;
            }
            break;
        case 2:
            switch (logType) {
                case 1:
                    sortSRT(0);
                    sortRR(1);
                    while(!data0.empty()||!data1.empty()){
                        threads.push_back(thread(schedule,2,1));
                    }
                    break;
                case 2:
                    sortSRT(0);
                    sortSRT(1);
                    while(!data0.empty()||!data1.empty()){
                        threads.push_back(thread(schedule,2,2));
                    }
                    break;
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    for(auto& thread : threads){
        thread.join();
    }
    /*printf("memory:\n");
    for(int i=0;i<memAns.size();i++)
    {
        printf("%d",memAns[i]);
    }*/
}
