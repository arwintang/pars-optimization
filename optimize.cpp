//
//  optimize.cpp
//  pars_optimize
//
//  Created by Feng Tang on 6/9/15.
//  Copyright (c) 2014 Fordham University. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "optimize.h"

void Optimize::readData(string filePath) {
    fstream inFile;
    inFile.open(filePath.c_str(), ios::in);
    
    if (!inFile.is_open()){
        cout << "Error opening file"<<endl;
        return;
    }
    
    vector<OptiEntry> tempOptiTable;
    string line;
    int index;
    while (getline(inFile, line))
    {
        istringstream iss(line);
        string a;
        double b;
        if (!(iss >> a >> b))
            break;
        else if(a.compare("minT")==0){
            minT = (int) b;
            minTime = minT;
        }
        else if(a.compare("maxT")==0){
            maxT = (int) b;
            maxTime = maxT;
        }
        else if(a.compare("Entry")==0){
            index = (int)b;
            
            OptiEntry newEntry;
            tempOptiTable.push_back(newEntry);
        }
        else{
            stringstream ss(a);
            int tempTime;
            ss >> tempTime;
            tempOptiTable.back().time.push_back(tempTime);
            tempOptiTable.back().prob.push_back(b);
        }
    }
    
    inFile.close();
    
    for(int i = 0; i < tempOptiTable.size(); i++){
        OptiEntry myEntry = tempOptiTable[i];
        OptiEntry newEntry;
        
        for (int j = myEntry.prob.size() - 1; j > 0; j--){
            if (myEntry.prob[j] == 1 && myEntry.prob[j-1] == 1) {
                myEntry.prob.pop_back();
                myEntry.time.pop_back();
            }
        }
        
        newEntry.prob = myEntry.prob;
        newEntry.time = myEntry.time;
        
        int size = myEntry.time.size();
        for(int j = 0; j < size-1; j++){
            newEntry.delp.push_back(myEntry.prob[j+1]-myEntry.prob[j]);
        }
        newEntry.delp.push_back(-1);
        
        if(size>=2){
            for(int j = 0; j < size-2; j++){
                newEntry.del2p.push_back(newEntry.delp[j+1] + newEntry.delp[j]);
            }
        }
        newEntry.del2p.push_back(0);
        newEntry.del2p.push_back(-1);
        
        optiTable.push_back(newEntry);
        optiTable[i].nonCumProb = optiTable[i].prob;
        for (int j = optiTable[i].prob.size() - 1; j > 0; j--){
            optiTable[i].nonCumProb[j] = optiTable[i].nonCumProb[j] - optiTable[i].nonCumProb[j-1];
        }
    }
    
    minT = 0;
    maxT = 0;
    for (int i=0; i< optiTable.size(); i++) {
        minT += optiTable[i].time[0];
        maxT += optiTable[i].time[optiTable[i].time.size()-1];
    }
    minTime = minT;
    maxTime = maxT;
}

void Optimize::cleanOptionsAux() {
    optionsAux.prob.clear();
    optionsAux.time.clear();
    optionsAux.history.clear();
}

bool Optimize::isTableReady() {
    return (optiTable.size()>0);
}


void Optimize::printTable(vector<OptiEntry> table){
    if (table.size()==0) {
        cerr<<"Fail to read table!"<<endl;
        return;
    }
    
    for(int i=0; i<table.size(); i++)
    {
        cerr<<"Entry: "<<i<<endl;
        OptiEntry myEntry = table[i];
        for(int j = 0; j < myEntry.prob.size(); j++){
            cerr<<myEntry.time[j]<<":"<<myEntry.prob[j]<<" "<<myEntry.nonCumProb[j]<<endl;
        }
    }
}

void Optimize::printTable(){
    if (optiTable.size()==0) {
        cerr<<"Fail to read table!"<<endl;
        return;
    }
    
    for(int i=0; i<optiTable.size(); i++)
    {
        cerr<<"Entry "<<i<<endl;
        OptiEntry myEntry = optiTable[i];
        for(int j = 0; j < myEntry.prob.size(); j++){
            //cerr<<myEntry.time[j]<<" "<<myEntry.prob[j]<<" "<<myEntry.delp[j]<<" "<<myEntry.del2p[j]<<endl;
            cerr<<myEntry.time[j]<<" "<<myEntry.prob[j]<<" "<<myEntry.nonCumProb[j]<<endl;
        }
    }
}


void Optimize::parseTable(vector<int> &singlePos, vector<OptiEntry> &singleTable, vector<OptiEntry> &multiTable){
    if (!isTableReady()) {
        cerr<<"Fail to read table!"<<endl;
        return;
    }
    
    for(int i=0; i<optiTable.size(); i++)
    {
        OptiEntry myEntry = optiTable[i];
        if(myEntry.prob.size()==1){
            singleTable.push_back(myEntry);
            singlePos.push_back(i);
        }
        else if(myEntry.prob.size()>1){
            multiTable.push_back(myEntry);
        }
    }
}

void Optimize::parseTable(vector<OptiEntry> table, vector<int> &singlePos, vector<OptiEntry> &singleTable, vector<OptiEntry> &multiTable){
    if (table.size() == 0) {
        cerr << "Fail to read table!" << endl;
        return;
    }
    
    for(int i = 0; i < table.size(); i++)
    {
        OptiEntry myEntry = table[i];
        if(myEntry.prob.size()==1){
            singleTable.push_back(myEntry);
            singlePos.push_back(i);
        }
        else if(myEntry.prob.size()>1){
            multiTable.push_back(myEntry);
        }
    }
}

//obtain best, worst and average probabilities at each timestep in the [tMin, tMax]
void Optimize::makeOptions(int tMin, int tMax){
    vector<OptiEntry> singleTable;
    vector<OptiEntry> multiTable;
    vector<int> singlePos;
    parseTable(singlePos, singleTable, multiTable);
    int singleTime = 0;
    double singleProb = 1.0;
    for(int i=0; i<singleTable.size(); i++){
        for(int j=0; j<singleTable[i].prob.size(); j++){
            singleTime += singleTable[i].time[j];
            singleProb = singleProb*singleTable[i].prob[j];
        }
    }
    int newTMIN = tMin - singleTime;
    int newTMax = tMax - singleTime;
    
    for(int i=newTMIN; i<=newTMax; i++){
        vector<int> hT;
        vector<double> hP;
        makeOptionsAux(0, multiTable, 1.0, 0, i, hP, hT);
        if (optionsAux.prob.size()==0) {
            cerr<<"Error in generate candidate probability at timestep "<<i+singleTime<<endl;
        }
        else{
            options.time.push_back(i+singleTime);
            
            int bestIndex = findBest(optionsAux.prob);
            int worstIndex = findWorst(optionsAux.prob);
            
            completeHistory(singlePos, optionsAux.history[bestIndex]);
            completeHistory(singlePos, optionsAux.history[worstIndex]);
            
            if(bestIndex>-1){
                options.bestProb.push_back(optionsAux.prob[bestIndex]*singleProb);
                options.bestHistory.push_back(optionsAux.history[bestIndex]);
            }
            if(worstIndex>-1){
                options.worstProb.push_back(optionsAux.prob[worstIndex]*singleProb);
                options.worstHistory.push_back(optionsAux.history[worstIndex]);
            }
            
            double avgProb = findAvg(optionsAux.prob);
            options.avgProb.push_back(avgProb*singleProb);
            
            cleanOptionsAux();
        }
    }
    
    for(int i = 0; i < multiTable.size(); i++){
        multiTable[i].prob = multiTable[i].nonCumProb;
    }
    
    for(int i=newTMIN; i<=newTMax; i++){
        vector<int> hT;
        vector<double> hP;
        makeOptionsAux(0, multiTable, 1.0, 0, i, hP, hT);
        if (optionsAux.prob.size()==0) {
            cerr<<"Error in generate candidate probability at timestep "<<i+singleTime<<endl;
        }
        else{
            double sumProb = findSum(optionsAux.prob);
            options.sumProb.push_back(sumProb*singleProb);
            cleanOptionsAux();
        }
    }
    
    if(options.sumProb.size() > 0){
        double sum = 0.0;
        for(int i = 0; i < options.sumProb.size(); i++){
            sum += options.sumProb[i];
        }
        
        for(int i = 0; i < options.sumProb.size(); i++){
            options.sumProb[i] = options.sumProb[i]/sum;
        }
        
        for(int i = 1; i < options.sumProb.size(); i++){
            options.sumProb[i] = options.sumProb[i] + options.sumProb[i-1];
        }
        
    }
    
    for(int i = 0; i < options.time.size(); i++){
        cerr << "Timestep: " << options.time[i];
        cerr << " Best Prob: " << options.bestProb[i];
        cerr << " Worst Prob: " << options.worstProb[i];
        cerr << " Average Prob: " << options.avgProb[i];
        cerr << " New Average Prob: " << options.sumProb[i] << endl;
    }
}

//generate the candidate probabilities at timestep tMax
void Optimize::makeOptionsAux(int level, vector<OptiEntry> table, double cProb, int cTime, int tMax, vector<double> hProb, vector<int> hTime) {
    
    if(level==table.size()){
        if(cTime==tMax){
            OptiEntry history;
            optionsAux.prob.push_back(cProb);
            optionsAux.time.push_back(cTime);
            history.time = hTime;
            history.prob = hProb;
            optionsAux.history.push_back(history);
        }
    }
    else{
        vector<int> myHTime = hTime;
        vector<double> myHProb = hProb;
        OptiEntry myLevel = table[level];
        for (int i=0; i<myLevel.prob.size(); i++){
            if (tMax>0 && cTime+myLevel.time[i]<=tMax){
                myHProb.push_back(myLevel.prob[i]);
                myHTime.push_back(myLevel.time[i]);
                makeOptionsAux(level+1, table, cProb*myLevel.prob[i],cTime+myLevel.time[i], tMax, myHProb, myHTime);
                myHProb.pop_back();
                myHTime.pop_back();
            }
        }
    }
    return;
}

//find the best probability from the candidate probability vector
int Optimize::findBest(vector<double> prob) {
    int bestIndex = -1;
    double bestProb = 0.0;
    
    for(int i=0; i<prob.size(); i++){
        if(prob[i] >= bestProb){
            bestIndex = i;
            bestProb = prob[i];
        }
    }
    return bestIndex;
}

//find the worst probability from the candidate probability vector
int Optimize::findWorst(vector<double> prob) {
    int worstIndex = -1;
    double worstProb = 1.0;
    
    for(int i=0; i<prob.size(); i++){
        if(prob[i] <= worstProb){
            worstIndex = i;
            worstProb = prob[i];
        }
    }
    return worstIndex;
}

//find the average probability from the candidate probability vector
double Optimize::findAvg(vector<double> prob) {
    double sumProb = 0.0;
    double avgProb = 0.0;
    
    for (int i=0; i<prob.size(); i++)
        sumProb += prob[i];
    
    avgProb = sumProb/prob.size();
    return avgProb;
}

//find the sum probability from the candidate probability vector
double Optimize::findSum(vector<double> prob) {
    double sumProb = 0.0;

    for (int i=0; i<prob.size(); i++)
        sumProb += prob[i];
    
    return sumProb;
}

void Optimize::completeHistory(vector<int> singlePos, OptiEntry &history){
    for(int i=0; i<singlePos.size(); i++){
        int index = singlePos[i];
        OptiEntry myEntry = optiTable[index];
        int time = myEntry.time[0];
        double prob = myEntry.prob[0];
        history.time.insert(history.time.begin()+index, time);
        history.prob.insert(history.prob.begin()+index, prob);
    }
}

void Optimize::outputResult(string filePath, string mode){
    ofstream outFile;
    if (!outFile.is_open())
        outFile.open(filePath.c_str(), ios::out);
    if(mode.compare("global")==0){
        outFile<<"Timestep,Best,Worst,Average,NewAverage"<<endl;
        for(int i=0; i<options.time.size(); i++)
        {
            outFile<<options.time[i]<<","<<options.bestProb[i]<<","<<options.worstProb[i]<<","<<options.avgProb[i]<<","<<options.sumProb[i]<<endl;
        }
    }
    if(mode.compare("local")==0){
        outFile<<"Timestep,Best,Worst"<<endl;
        for(int i=0; i<options.time.size(); i++)
        {
            outFile<<options.time[i]<<","<<options.bestProb[i]<<","<<options.worstProb[i]<<endl;
        }
    }
}

bool Optimize::evalSolution(vector<int> sol, int &sTime, double &sProb){
    if(sol.size() != optiTable.size())
        return false;
    
    sTime = 0;
    sProb = 1;
    
    for(int i = 0; i < sol.size(); i++){
        if(sol[i] > optiTable[i].prob.size())
            return false;
        else{
            sTime += optiTable[i].time[sol[i]];
            sProb *= optiTable[i].prob[sol[i]];
        }
    }
    return true;
}

#define MAX(a,b) (a)>(b)?(a):(b)

int Optimize::findBestDel(vector<int> sol){
    int bestIndex = -1;
    double bestDel = -1;
    for(int i = 0; i < sol.size(); i++){
        if(optiTable[i].delp[sol[i]] + optiTable[i].del2p[sol[i]] > bestDel ){
            bestDel = optiTable[i].delp[sol[i]] + optiTable[i].del2p[sol[i]];
            bestIndex = i;
        }
    }
    if(bestIndex==-1)
        return -1;
    else
        return bestIndex;
}

int Optimize::findWorstDel(vector<int> sol) {
    int worstIndex = -1;
    double  worstDel = 10.0;
    for (int i = 0; i < sol.size(); i++) {
        double est = optiTable[i].delp[sol[i]] + optiTable[i].del2p[sol[i]];
        if(est > 0 && est < worstDel && optiTable[i].prob.size() > sol[i]){
            worstDel = est;
            worstIndex = i;
        }
    }
    if (worstDel==1.0)
        return -1;
    else
        return worstIndex;
}

bool Optimize::findBest2(int maxT, double &optiProb, int &optiTime) {
    vector<int> solution;
    vector<int> newSolution;
    int solutionTime;
    double solutionProb;
    
    // first approximation is to set every problem to lowest time
    for (int i = 0; i < optiTable.size(); i++)
        solution.push_back(0);
    
    // and figure out if that is okay
    evalSolution(solution, solutionTime, solutionProb);
    
    if (solutionTime > maxT)
        return false; // no solution possible
    
    int bestDel =0;
    while (solutionTime <= maxT && bestDel >= 0) {
        bestDel = findBestDel(solution); // best one to change
        if (bestDel >= 0) {
            newSolution = solution;
            if (optiTable[bestDel].prob.size() > newSolution[bestDel]) {
                newSolution[bestDel] += 1;
                evalSolution(newSolution, solutionTime, solutionProb);
                if (solutionTime <= maxT)
                    solution = newSolution;
            }
            else {
                bestDel=-1;
            }
        }
    }
    evalSolution(solution, solutionTime, solutionProb);
    optiProb = solutionProb;
    optiTime = solutionTime;
    
    return true;
}

bool Optimize::findWorst2(int maxT, double &optiProb, int &optiTime) {
    vector<int> solution;
    vector<int> newSolution;
    int solutionTime;
    double solutionProb;
    
    
    // first approximation is to set every problem to lowest time
    for (int i=0; i<optiTable.size(); i++)
        solution.push_back(0);
    
    // and figure out if that is okay
    evalSolution(solution, solutionTime, solutionProb);
    
    //if (solutionTime<maxT) return false; // no solution possible
    
    int worstDel =0;
    while (solutionTime <= maxT && worstDel >= 0) {
        worstDel = findWorstDel(solution); // best one to change
        if (worstDel>=0) {
            newSolution = solution;
            if (optiTable[worstDel].prob.size() > newSolution[worstDel]) {
                newSolution[worstDel] += 1;
                evalSolution(newSolution, solutionTime, solutionProb);
                if (solutionTime <= maxT)
                    solution = newSolution;
            }
            else {
                worstDel=-1;
            }
        }
    }
    
    evalSolution(solution, solutionTime, solutionProb);
    optiProb = solutionProb; 
    optiTime = solutionTime;
    
    return true;
}

void Optimize::makeLocalOptions(int tMin, int tMax){
    int bestTime, worstTime;
    double bestProb, worstProb;
    
    if(!isTableReady()){
        cerr<<"Fail to read table!"<<endl;
        return;
    }
    
    int index = tMin;
    do {
        options.time.push_back(index);
        cerr<<"Timestep: "<<index;
        if(findBest2(index, bestProb, bestTime)){
            cerr<<" Best prob: "<<bestProb;
            options.bestProb.push_back(bestProb);
        }
        if(findWorst2(index, worstProb, worstTime)){
            cerr<<" Worst prob: "<<worstProb<<endl;
            options.worstProb.push_back(worstProb);
        }
        
    } while(index++ <= tMax);
    
}



