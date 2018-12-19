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

void feed_voc(string file, map<string,float>& voc ){
    string word,val;
    ifstream in;
    in.open(file);
    while(in >> word >> val) {
        voc.insert(pair<string,float>(word,stof(val)));
    }
    in.close();
}

void feed_tweets(string file, map<int,tweet>& tweets ){
    vector<string> temp;
    tweet twt;
    string word,line;
    ifstream in;
    int i,uid,twid;
    in.open(file);
    while(getline(in,line)) {
        istringstream iss(line);
        vector<string> temp;
        i=0;
        while (getline(iss,word,'\t')) {
            if(i==0){ uid=stoi(word);}
            else if(i==1){ twid=stoi(word);}
            else{
                temp.push_back(word);
            }
            i++;
        }
        twt.tweet_id=twid;
        twt.user_id=uid;
        twt.tokens=temp;
        tweets.insert(pair<int,tweet>(twid,twt));
        temp.clear();
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
    map<string,float> voc;
    map<int,tweet> raw_tweets;

    feed_data_set(consts::input_file,vec_tweets,dim,num_lines);
    feed_coins(consts::coins,coinz);
    feed_voc(consts::voc,voc);
    feed_tweets(consts::tweets,raw_tweets);
    for (map<int,tweet>::iterator it=raw_tweets.begin();it!=raw_tweets.end();it++){
        cout << "tweet id: "<<it->first << " user id: " << it->second.user_id << " len: " << it->second.tokens.size()<<endl;
    }
    return 0;
}