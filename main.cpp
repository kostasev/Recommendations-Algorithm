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

void user_feels(map<int,tweet> raw_tweets, map<int,data_point<double>>& feels,vector<string>* coinz, map<string,float> voc){
    data_point<double> temp;
    temp.sum=0;

    for(int i=0;i<100;i++)
        temp.point.push_back(0.0);
    map<int,tweet>::iterator it;
    for ( it=raw_tweets.begin();it!=raw_tweets.end();it++ ){
        temp.name=to_string(it->second.user_id);
        feels.insert(pair<int,data_point<double >>(it->second.user_id,temp));
    }
    vector<double> tmp;
    for( it =raw_tweets.begin(); it!=raw_tweets.end();it++ ){
        tmp=calc_feeling(it->second.tokens, voc, coinz);
        add_vectors(feels[it->second.user_id].point,tmp);
        tmp.clear();
    }
}

void random_vector(vector<int> &r,int k ){
    std::mt19937 generator;
    generator.seed(std::default_random_engine()());
    std::uniform_int_distribution<int>   int_dist(-99,99);
    for(int i=0; i<k ; i++){
        r.push_back(int_dist(generator));
    }
}

int create_tables(vector<Hash_table> &tables,string metric,int num_lines,int dim){
    int table_size;
    if (metric=="euclidean")
        table_size = num_lines/const_lsh::table_size;
    else
        table_size = (int) pow(2.0,const_lsh::k);

    for (int i=0; i<const_lsh::L ; i++){
        tables.push_back(Hash_table(table_size, dim, const_lsh::k, metric));
    }
    return table_size;
}

void feed_tables(vector<Hash_table> &tables,data_point<double> *data_set,int table_size,int num_lines,vector<int> r){
    for (int j=0;j<tables.size();j++) {
        for (int i = 0; i < num_lines; i++) {
            tables[j].add_item(data_set[i],table_size,r);
        }
    }
}

double cosine_similarity(vector<double> p1,vector<double> p2)
{
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
    for(unsigned int i = 0; i < p1.size(); i++) {
        dot += p1[i] * p2[i] ;
        denom_a += p1[i] * p1[i] ;
        denom_b += p2[i] * p2[i] ;
    }
    if(denom_a!=0.0&&denom_b!=0.0)
        return dot / (sqrt(denom_a) * sqrt(denom_b)) ;
    else
        return 0.0;
}

struct recom{
    double value;
    int coin;
};


vector<recom> rec_nn(map<string,value_point<double>> bucks, data_point<double> point) {
    vector<recom> recomm(100), fail;
    for(int i=0;i<100;i++){
        recomm[i].value=point.point[i];
        recomm[i].coin=i;
    }
    if(bucks.size()==0){
        return fail;
    }
    double cosine;
    for (auto  it = bucks.begin(); it != bucks.end(); it++ ){
        cosine=cosine_similarity(point.point,it->second.p->point);
        for(int i=0; i<100;i++){
            recomm[i].value+=cosine*it->second.p->point[i];
        }
    }
    for (int i=0 ; i < recomm.size(); i++){
        recomm[i].value*=consts::z;
    }
    return recomm;
}

bool myfunction (recom i,recom j) { return (i.value<j.value); }

bool in_init(int coin,data_point<double> data){
    return (data.point[coin]>0.0);
}


void print_recom(vector<recom> recomm,data_point<double> data,int coins,vector<string>* cn){
    int i=99;
    vector<int> coins_ids;
    while((coins>0)&&(i>0)){
        if(in_init(recomm[i].coin,data)){
            i--;
            continue;
        }
        else{
            coins_ids.push_back(recomm[i].coin);
            coins--;
            i--;
        }
    }
    if (coins==0) {
        cout << "<u"+data.name+"> ";
        for(int i=0;i<coins_ids.size();i++)
            cout << cn[coins_ids[i]][0] << " " ;
        cout << endl;
    }
    return;
}


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

