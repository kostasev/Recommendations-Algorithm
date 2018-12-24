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

int main(int argc, char** argv) {
    int c, num_lines=0, dim=0;
    string input="", output="";
    while ((c = getopt(argc, argv, "i:o:h")) != -1) {
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
    map<string,float> voc;
    map<int,tweet> raw_tweets;
    map<int,data_point<double>> feels;

    feed_data_set(consts::input_file,vec_tweets,dim,num_lines);
    feed_coins(consts::coins,coinz);
    feed_voc(consts::voc,voc);
    feed_tweets(consts::tweets,raw_tweets);

    user_feels( raw_tweets, feels,coinz, voc);
    cluster_feels(raw_tweets,feels,coinz,voc);
    print_feels(feels);
    return 0;
}

