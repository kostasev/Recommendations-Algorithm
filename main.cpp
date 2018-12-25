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
#include <algorithm>
#include "constants.h"
#include "utilities.h"
#include "data_point.h"
#include "Hash_table.h"
#include "Value.h"
#include "Key.h"

using namespace std;

bool myfunction (recom i,recom j) { return (i.value<j.value); }


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
    map<int,data_point<double>>::iterator fit;

    feed_data_set(consts::input_file,vec_tweets,dim,num_lines);
    feed_coins(consts::coins,coinz);
    feed_voc(consts::voc,voc);
    feed_tweets(consts::tweets,raw_tweets);

    user_feels( raw_tweets, feels,coinz, voc);
    data_point<double> temp;
    int users_feels = feels.size();
    //cluster_feels(raw_tweets,feels,coinz,voc);
    //int cluster_feels=feels.size()-users_feels;
    //print_feels(feels);

    /* Cosine LSH Recommendation */
    //5 best Coins
    int table_size,i=0;
    vector<int> r;
    vector<Hash_table> tables,tables2 ;
    data_point<double> data_set[users_feels];
    for (fit=feels.begin();fit!=feels.end();fit++){
        data_set[i]=fit->second;
        i++;
    }
    random_vector(r,const_lsh::k);
    table_size=create_tables(tables,"cosine",users_feels,dim);
    feed_tables(tables,data_set,table_size,users_feels,r);

    map<string, value_point<double>> bucks;
    vector<recom> recomm;
    Key query_key;
    for (int k = 0; k < users_feels; k++) {
        for (int i=0;i<tables.size();i++){
            query_key=tables[i].query_item(data_set[k],table_size,r);
            tables[i].get_bucket(data_set[k],query_key, bucks,r);
        }
        if (bucks.size()==0) continue;
        recomm=rec_nn(bucks,data_set[k]);
        sort(recomm.begin(),recomm.end(),myfunction);
        print_recom(recomm,data_set[k],5,coinz);
        recomm.clear();
        bucks.clear();
    }
    tables.clear();
    feels.clear();
    cluster_feels(raw_tweets,feels,coinz,voc);
    int cluster_feels=feels.size();
    /* Cosine LSH Recommendation */

    //2 best Coins from clusters
    data_point<double> data_set2[cluster_feels];
    i=0;
    for (fit=feels.begin();fit!=feels.end();fit++){
        data_set2[i]=fit->second;
        i++;
    }
    r.clear();
    random_vector(r,const_lsh::k);
    table_size=create_tables(tables2,"cosine",cluster_feels,dim);
    feed_tables(tables2,data_set2,table_size,cluster_feels,r);

    for (int k = 0; k < users_feels; k++) {
        for (int i=0;i<tables2.size();i++){
            query_key=tables2[i].query_item(data_set[k],table_size,r);
            tables2[i].get_bucket(data_set[k],query_key, bucks,r);
        }
        if (bucks.size()==0) continue;
        recomm=rec_nn(bucks,data_set[k]);
        sort(recomm.begin(),recomm.end(),myfunction);
        print_recom(recomm,data_set[k],2,coinz);
        recomm.clear();
        bucks.clear();
    }

    return 0;
}

