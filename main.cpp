#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <getopt.h>
#include <cstring>
#include <numeric>
#include <cmath>
#include <chrono>
#include <random>
#include <unistd.h>
#include <sstream>
#include <map>
#include "constants.h"
#include "utilities.h"
#include "data_point.h"

using namespace std;

void feed_coins(string file, vector<string>* coins ){
    vector<string> temp;
    string word,line;
    ifstream in;
    int i = 0 ;
    in.open(file);
    while(getline(in,line)) {
        istringstream iss(line);
        vector<string> temp;
        while (getline(iss,word,',')) {
            temp.push_back(word);
        }
        coins[i]=temp;//.push_back(temp);
        i++;
        temp.clear();
    }
    in.close();
}

void feed_voc(string file, map<string,double>& voc ){
    string word,val;
    ifstream in;
    in.open(file);
    while(in >> word >> val) {
        voc.insert(pair<string,double>(word,stod(val)));
    }
    in.close();
}

int main(int argc, char** argv) {
    int c, num_lines=0, dim=0;
    string input="", output="";
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
        switch (c) {
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'h':
                cout << "\nRecommendation\n" << endl <<
                     "Mandatory options\n\t-i -o\n\n"
                     "\t-i input file\n"
                     "\t-o output file\n";
                return 0;
            case '?':
                cerr << "Unknown option character " << char(optopt) << endl;
                return 1;
            default:
                abort();
        }
    }
    get_data_lengths(consts::input_file,num_lines,dim);
    data_point<double> vec_tweets[num_lines];
    vector<string> coinz[100];
    map<string,double> voc;

    feed_data_set(consts::input_file,vec_tweets,dim,num_lines);
    feed_coins(consts::coins,coinz);
    feed_voc(consts::voc,voc);

    return 0;
}