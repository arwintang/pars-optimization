# PARS Optimization
## This code is built to calculate the best, average and worst probabilities for each time step after obtaining time-prob table generated by VIPARS
## Running time optimization
### Generate time result:
#### To compile:
    cd optimize/
    make clean
    make optimize

#### To run, simply type:
    ./optimize <arg> <path to input file>  <path to output file> 

where <arg> = -global | -local

'-global' for Global Optimization and '-local' for Local Optimization

If we don't input <path to input file>, the default name is "table.txt";

If we don't input <path to output file>, the default name is "result.cv";

### Merge multiple tables

Run VIPARS k times with different probability: p1, p2, ..., pk. Then we obtained k time-prob tables. Name each table as table_pi.txt, i = 1, 2, ..., k, (or arbitrary name). Put these tables in the same folder

#### To compile:
    cd optimize/
    make clean
    make merge

#### To run, simply type:
    ./merge <path to input folder>  <path to output file> 

If we don't input <path to input folder>, the default name is "tables";

If we don't input <path to output file>, the default name is "table.txt";

After the merged table is generated, use optimize function to get the time result.
