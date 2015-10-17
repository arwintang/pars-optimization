//
//  main.cpp
//  ParsMergeTables
//
//  Created by Feng Tang on 10/15/15.
//  Copyright © 2015 Feng Tang. All rights reserved.
//

#include <stdlib.h>

#include "merge.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string tables_folder_path = "/Users/feng/Documents/Workspace/cpp/ParsMergeTables/tables";
    //string tables_folder_path = "tables";
    string output_table_path = "/Users/feng/Documents/Workspace/cpp/ParsMergeTables/table.txt";
    //string output_table_path = "table.txt";

    switch (argc) {
        case 1: {
            break;
        }
        case 2: {
            tables_folder_path = argv[1];
            break;
        }
        case 3: {
            tables_folder_path = argv[1];
            output_table_path = argv[2];
            break;
        }
        default: {
            cout << "The command had wrong number of arguments." << endl;
            return 0;
        }
    }
    
    string list_file_path = "/Users/feng/Documents/Workspace/cpp/ParsMergeTables/list.txt";
    //string list_file_path = "list.txt";
    string ls_command = "ls " + tables_folder_path +  " > " + list_file_path;
    int sys_return = system(ls_command.c_str());
    if (sys_return != 0) {
        cout << "Fail to read the list of tables" << endl;
        return 0;
    }
    
    fstream in_file;
    in_file.open(list_file_path.c_str(), ios::in);
    if (!in_file.is_open()){
        cout << "Error in opening table list" << endl;
        return 0;
    }

    MultiTable final_table = MultiTable();
    string line;
    while (getline(in_file, line)) {
        string table_path = tables_folder_path + "/" + line;
        SingleTable single_table = SingleTable();
        single_table.readData(table_path.c_str());
        if (!final_table.mergeSingleTable(single_table)) {
            cout << "Different size of the tables" << endl;
            return 0;
        }
    }
    string output_whole_table_path = "/Users/feng/Documents/Workspace/cpp/ParsMergeTables/table_whole.txt";
    final_table.sortTable();
    final_table.outputToTable(output_whole_table_path);
    final_table.processTable();
    final_table.outputToTable(output_table_path);
    
    string rm_command = "rm " + list_file_path;
    sys_return = system(rm_command.c_str());
    if (sys_return == 0) {
       cout << "List of tables is deleted" << endl;
    }

    return 0;
}
