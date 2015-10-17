//
//  optimize.h
//  pars_optimize
//
//  Created by Feng Tang on 6/9/15.
//  Copyright (c) 2015 Fordham University. All rights reserved.
//

#ifndef optimize_h
#define optimize_h

#include <vector>

using namespace std;

class OptiEntry {
public:
    vector<int> time;
    vector<double> prob;
    vector<double> nonCumProb;
    vector<double> delp;
    vector<double> del2p;
    
    OptiEntry() {
        prob.clear();
        time.clear();
        delp.clear();
        del2p.clear();
    }
    ~OptiEntry() {}
};

class OptionEntry {
public:
    vector<int> time;
    vector<double> bestProb;
    vector<double> worstProb;
    vector<double> avgProb;
    vector<double> sumProb;
    
    vector<OptiEntry> bestHistory;
    vector<OptiEntry> worstHistory;
    
    OptionEntry() {
        time.clear();
        bestProb.clear();
        worstProb.clear();
        avgProb.clear();
        bestHistory.clear();
        worstHistory.clear();
    }
    ~OptionEntry(){}
};

class OptionEntryAux {
public:
    vector<double> prob;
    vector<int> time;
    vector<OptiEntry> history;
    OptionEntryAux() {
        prob.clear();
        time.clear();
        history.clear();
    }
    ~OptionEntryAux(){}
};


class Optimize{
    int minT;
    int maxT;
    vector<OptiEntry> optiTable;
    OptionEntry options;
    OptionEntryAux optionsAux;
    
public:
    int minTime;
    int maxTime;
    Optimize(){
        optiTable.clear();
    }
    ~Optimize() {}
    
    void readData(string filePath);
    void cleanOptionsAux();
    bool isTableReady();
    
    void printTable(vector<OptiEntry> table);
    void printTable();
    void parseTable(vector<int> &singlePos, vector<OptiEntry> &singleTable, vector<OptiEntry> &multiTable);
    void parseTable(vector<OptiEntry> table, vector<int> &singlePos, vector<OptiEntry> &singleTable, vector<OptiEntry> &multiTable);
    
    void makeOptions(int tMin, int tMax);
    void makeOptionsAux(int level, vector<OptiEntry> table, double cProb, int cTime, int tMax,
                        vector<double> hProb, vector<int> hTime);
    
    int findBest(vector<double> prob);
    int findWorst(vector<double> prob);
    double findAvg(vector<double> prob);
    double findSum(vector<double> prob);
    
    void completeHistory(vector<int> singlePos, OptiEntry &history);
    void outputResult(string filePath, string mode);
    
    bool evalSolution(vector<int> sol, int &sTime, double &sProb);
    int findBestDel(vector<int> sol);
    int findWorstDel(vector<int> sol);
    bool findBest2(int maxT, double &optiProb, int &optiTime);
    bool findWorst2(int maxT, double &optiProb, int &optiTime);
    
    void makeLocalOptions(int tMin, int tMax);
};

#endif
