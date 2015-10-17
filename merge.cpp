//
//  merge.cpp
//  ParsMergeTables
//
//  Created by Feng Tang on 10/15/15.
//  Copyright © 2015 Feng Tang. All rights reserved.
//

#include "merge.h"


void SingleTable::readData(std::string file_path) {
    std::fstream in_file;
    in_file.open(file_path.c_str(), std::ios::in);
    
    if (!in_file.is_open()){
        std::cout << "Error int opening single table file" << std::endl;
        return;
    }
    
    std::string line;
    int index;
    while (getline(in_file, line)) {
        std::istringstream iss(line);
        std::string first;
        std::string second;
        if (!(iss >> first >> second)) {
            break;
        } else if(first == "minT"){
            min_time = stoi(second);
        } else if(first == "maxT"){
            max_time = stoi(second);
        } else if(first == "Entry"){
            index = stoi(second);
            std::cout << "Entry: " << index << std::endl;
        } else {
            int time = stoi(first);
            double prob = stod(second);
            std::pair<int, double> new_pair = std::make_pair(time, prob);
            entries.push_back(new_pair);
        }
    }
    in_file.close();
}

bool MultiTable::mergeSingleTable(SingleTable single_table) {
    min_time = std::min(min_time, single_table.getMinTime());
    max_time = std::max(max_time, single_table.getMaxTime());
    std::vector<std::pair<int, double>> new_entries = single_table.getEntyies();
    
    if (size == 0) { // Initializa multi_entries
        size = static_cast<int>(new_entries.size());
        for (int i = 0; i < size; i++) {
            std::vector<std::pair<int, double>> multi_entry;
            multi_entries.push_back(multi_entry);
        }
    } else if (size != static_cast<int>(new_entries.size())) {
        std::cout << "Entry size is wrong!" << std::endl;
        return false;
    }
    for (int i = 0; i < size; i++) {
        multi_entries[i].push_back(new_entries[i]);
    }
    return true;
}

void MultiTable::sortTable() {
    if (multi_entries.size() == 0) {
        std::cout << "No data, fail to sort table" << std::endl;
        return;
    }
    for(int i = 0; i < multi_entries.size(); i++) {
        std::sort(multi_entries[i].begin(), multi_entries[i].end());
    }
}

void MultiTable::outputToTable(std::string file_path) {
    if (multi_entries.size() == 0) {
        std::cout << "No data, fail to output table" << std::endl;
        return;
    }
    //sortTable();
    std::ofstream out_file;
    if (!out_file.is_open()) {
        out_file.open(file_path.c_str());
    }
    out_file << "minT " << min_time << "\n";
    out_file << "maxT " << max_time << "\n";
    for(int i = 0; i < multi_entries.size(); i++) {
        out_file << "Entry " << i << "\n";
        for(int j = 0; j < multi_entries[i].size(); j++){
            out_file << multi_entries[i][j].first << " " << multi_entries[i][j].second << "\n";
        }
    }
}

void MultiTable::processTable() {
    if (multi_entries.size() == 0) {
        std::cout << "No data, fail to process table" << std::endl;
        return;
    }
    //sortTable();
    for(int i = 0; i < multi_entries.size(); i++) {
        std::unordered_map<int, std::pair<int, double>> map;
        for(int j = 0; j < multi_entries[i].size(); j++) {
            if (map.find(multi_entries[i][j].first) == map.end()) {
                map[multi_entries[i][j].first] = std::make_pair(1, multi_entries[i][j].second);
            } else {
                map[multi_entries[i][j].first].first++;
                map[multi_entries[i][j].first].second += multi_entries[i][j].second;
            }
        }
        multi_entries[i].clear();
        for (auto& x : map) {
            std::pair<int, double> new_pair = std::make_pair(x.first, x.second.second/x.second.first);
            multi_entries[i].push_back(new_pair);
        }
    }
    
    sortTable();
    for(int i = 0; i < multi_entries.size(); i++) {
        for (int j = 1; j < multi_entries[i].size(); j++) {
            if (multi_entries[i][j - 1].second > multi_entries[i][j].second) {
                multi_entries[i][j].second = multi_entries[i][j - 1].second;
            }
        }
    }
    
    for(int i = 0; i < multi_entries.size(); i++) {
        for (int j = 1; j < multi_entries[i].size(); j++) {
            if (multi_entries[i][j - 1].first + 1 < multi_entries[i][j].first) {
                multi_entries[i].insert(multi_entries[i].begin() + j,
                                        std::make_pair(multi_entries[i][j - 1].first + 1,
                                                       multi_entries[i][j - 1].second));
                
            }
        }
    }
    
}

