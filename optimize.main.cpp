//
//  optimize.main.cpp
//  pars_optimize
//
//  Created by Feng Tang on 6/9/15.
//  Copyright (c) 2015 Fordham University. All rights reserved.
//

#include <iostream>
#include <string>

#include "optimize.h"

int main(int argc, const char * argv[]) {
    
    string optiMethod;
    string inFilePath = "table.txt";
    string outFilePath = "result.csv";
    
    switch (argc) {
        case 2:
            optiMethod = argv[1];
            break;
        case 3:
        {
            optiMethod = argv[1];
            inFilePath = argv[2];
        }
            break;
        case 4:
        {
            optiMethod = argv[1];
            inFilePath = argv[2];
            outFilePath = argv[3];
        }
            break;
            
        default:
        {
            cout<<"The command had wrong number of arguments."<<endl;
            return 0;
        }
    }
    
    
    Optimize myOptimize;
    
    //read table
    myOptimize.readData(inFilePath);
    
    //print table
    cout<<"Table is as following:"<<endl;
    myOptimize.printTable();
    
    //global optimization
    if(optiMethod.compare("-global")==0){
        cout<<"Global Optimization:"<<endl;
        myOptimize.makeOptions(myOptimize.minTime, myOptimize.maxTime);
        myOptimize.outputResult(outFilePath, "global");
    }
    
    //local optimization
    else if(optiMethod.compare("-local")==0){
        cout<<"Local Optimization:"<<endl;
        myOptimize.makeLocalOptions(myOptimize.minTime, myOptimize.maxTime);
        myOptimize.outputResult(outFilePath, "local");	
    }
    
    else{
        cout<<"Parameter should be '-global' for Global Optimization and '-local' for Local Optimiztion"<<endl;
    }
    
    return 0;
}
