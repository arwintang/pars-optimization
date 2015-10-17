//
//  merge.h
//  ParsMergeTables
//
//  Created by Feng Tang on 10/15/15.
//  Copyright © 2015 Feng Tang. All rights reserved.
//

#ifndef MERGE_H_
#define MERGE_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

/*
struct Entry {
    int time;
    double prob;
    Entry(int t, double p) : time(t), prob(p) {}
};
 */

class SingleTable {
private:
    int min_time;
    int max_time;
    std::vector<std::pair<int, double>> entries;
    
public:
    SingleTable() {
        entries.clear();
    }
    int getMinTime() {
        return this->min_time;
    }
    void setMinTime(int min_time) {
        this->min_time = min_time;
    }
    int getMaxTime() {
        return this->max_time;
    }
    void setMaxTime(int max_time) {
        this->max_time = max_time;
    }
    std::vector<std::pair<int, double>> getEntyies() {
        return this->entries;
    }
    void setEntyies(std::vector<std::pair<int, double>> entries) {
        this->entries = entries;
    }
    void readData(std::string file_path);
};

class MultiTable {
private:
    int min_time;
    int max_time;
    int size;
    std::vector<std::vector<std::pair<int, double>>> multi_entries;
public:
    MultiTable() {
        min_time = INT_MAX;
        max_time = INT_MIN;
        size = 0;
        multi_entries.clear();
    }
    int getMinTime() {
        return this->min_time;
    }
    void setMinTime(int min_time) {
        this->min_time = min_time;
    }
    int getMaxTime() {
        return this->max_time;
    }
    void setMaxTime(int max_time) {
        this->max_time = max_time;
    }
    std::vector<std::vector<std::pair<int, double>>> getEntyies() {
        return this->multi_entries;
    }
    void setEntyies(std::vector<std::vector<std::pair<int, double>>> multi_entries) {
        this->multi_entries = multi_entries;
    }
    bool mergeSingleTable(SingleTable single_table);
    void sortTable();
    void processTable();
    void outputToTable(std::string file_path);
};


#endif /* MERGE_H_ */
