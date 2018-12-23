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

void add_vectors(vector<double>& v1,vector<double> v2){
    for(int i=0;i<v1.size();i++)
        v1[i]+=v2[i];
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

void mean_vec(vector<double> vec,int sum){
    for(int i=0;i<vec.size();i++){
        vec[i]/=sum;
    }
}

void mean_all(map<int,data_point<double>>& feels){
    map<int,data_point<double>>::iterator it;
    for(it=feels.begin();it!=feels.end();it++){
        mean_vec(it->second.point,it->second.sum);
    }
}


bool is_mentioned(vector<string> v1,vector<string> v2){
    for (int i=0;i<v1.size();i++){
        for (int j=0;j<v2.size();j++){
            if (v1[i]==v2[j])
                return true;
        }
    }
    return false;
}


vector<double> calc_feeling(vector<string> tweet ,map<string,float> voc ,vector<string> * coins){
    vector<double> feel(100,0.0);
    double sum;
    int flag=0;
    map<string,float>::iterator it;
    for(int i=0;i<tweet.size();i++){
        it = voc.find(tweet[i]);
        if (it != voc.end())
            sum+=voc[tweet[i]];
    }
    sum=sum/sqrt((sum*sum)+15);
    for(int i=0;i<100;i++){ //for each coin
        if(is_mentioned(coins[i],tweet)){
            feel[i]=sum;
        }
    }
    return feel;
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
    map<int,data_point<double>> feels;

    feed_data_set(consts::input_file,vec_tweets,dim,num_lines);
    feed_coins(consts::coins,coinz);
    feed_voc(consts::voc,voc);
    feed_tweets(consts::tweets,raw_tweets);


    //Start -- Read Feels for users
    data_point<double> temp;
    temp.sum=0;

    for(int i=0;i<100;i++)
        temp.point.push_back(0.0);
    map<int,tweet>::iterator it;
    for ( it=raw_tweets.begin();it!=raw_tweets.end();it++ ){
        feels.insert(pair<int,data_point<double >>(it->second.user_id,temp));
    }
    vector<double> tmp;
    for( it =raw_tweets.begin(); it!=raw_tweets.end();it++ ){
        tmp=calc_feeling(it->second.tokens, voc, coinz);
        add_vectors(feels[it->second.user_id].point,tmp);
        tmp.clear();
    }
    //End -- Read feels of users

    //Start -- Read Feelings of clusters
    ifstream cluster;
    string word,line;
    cluster.open(consts::clust_50);
    vector<int> cl_items;
    while(getline(cluster,line)){
        int clust_id=stoi(line);
        feels.insert(pair<int,data_point<double >>(clust_id,temp));
        getline(cluster,line);
        istringstream iss(line);
        while (getline(iss,word,' ')) {
            int post_id = stoi(word);
            tweet twt=raw_tweets[post_id];
            tmp=calc_feeling(twt.tokens, voc, coinz);
            add_vectors(feels[clust_id].point,tmp);
            tmp.clear();
        }
    }
    map<int,data_point<double>>::iterator fit;
    for(fit=feels.begin();fit!=feels.end();fit++){
        cout << "User: " << fit->first <<endl;
        for(int i=0;i<fit->second.point.size();i++) {
            cout << fit->second.point[i] << " " ;
        }
        cout << endl;
        if(fit->first==0)
            break;
    }
    return 0;
}
