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
#include "cluster.h"

using namespace std;

bool myfunction (recom i,recom j) { return (i.value<j.value); }

double euclidean_dist(vector<double> p1,vector<double> p2) {
    double sum = 0.0;
    for (int i=0 ; i<p1.size();i++){
        sum+=(p1[i]-p2[i])*(p1[i]-p2[i]);
    }
    sum = sqrt(sum);
    return sum;
}

void assign_to_clusters(data_point<double> *dat,vector<cluster> &clusters,int num_lines,string metric){
    double dist_min,dist;
    int cluster;
    for (int i=0;i<num_lines;i++){
        dist_min=100000.0;
        cluster=0;
        for(int j=0;j<clusters.size();j++){
            if (metric == "euclidean")
                dist=euclidean_dist(dat[i].point,clusters[j].get_centroid().point);
            else
                dist=cosine_similarity(dat[i].point,clusters[j].get_centroid().point);
            if (dist<dist_min){
                dist_min=dist;
                cluster=j;
            }
        }
        clusters[cluster].add_item(dat[i]);
    }
}

vector<double> calculate_mean_centroid(cluster cl) {
    vector<data_point<double>> cluster_dat = cl.get_items();
    int size = (int) cluster_dat.size();
    int len = 0;
    if(size > 0 )
        len=cluster_dat[0].point.size();
    vector<double> new_cl(100, 0.0);

    if(size > 0 && len == 100){
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < size; j++) {
                new_cl[i] += cluster_dat[j].point[i];
            }
        }

        for (int i = 0; i < 100; i++){
            new_cl[i]/=size;
        }
    }
    return new_cl;
}


vector<cluster> create_kmeans_centroids(data_point<double> *dat,int k,int length,string metric){
    double dist_max,dist,dist_min;
    data_point<double> temp_cent,temp_cent2;
    vector<cluster> clusters1;
    std::random_device rd; // assume unsigned int is 32 bits
    std::mt19937_64 generator(rd()); // seeded with 256 bits of entropy from random_device
    std::uniform_int_distribution<int>   uint_dist(0,length-1);
    cluster temp1=cluster(dat[uint_dist(generator)]);
    clusters1.push_back(temp1);
    temp_cent = temp1.get_centroid();
    while (--k>0){
        dist_max=0.0;
        for(int i=0 ; i < length ;i++){
            dist_min=100000.0;
            for(int j=0;j<clusters1.size();j++){
                if(metric=="euclidean")
                    dist=euclidean_dist(dat[i].point,clusters1[j].get_centroid().point);
                else
                    dist=cosine_similarity(dat[i].point,clusters1[j].get_centroid().point);
                if (dist<dist_min){
                    dist_min=dist;
                    temp_cent=dat[i];
                }
            }
            if (dist_min>dist_max){
                dist_max=dist_min;
                temp_cent2=temp_cent;
            }
        }
        clusters1.emplace_back(temp_cent2);
    }
    return clusters1;
}

vector<cluster> create_random_centroids(data_point<double> *dat,int k,int length){
    int flag=0;
    vector<cluster> clusters1;
    std::random_device rd; // assume unsigned int is 32 bits
    std::mt19937_64 generator(rd()); // seeded with 256 bits of entropy from random_device
    std::uniform_int_distribution<int>   uint_dist(0,length-1);
    for(int i=0 ; i < k ;i++){
        cluster temp1=cluster(dat[uint_dist(generator)]);
        for (int j=0;j<clusters1.size();j++){
            if(clusters1[j].get_centroid().name==temp1.get_centroid().name){
                i--;
                flag=1;
                break;
            }
        }
        if (flag==0)
            clusters1.push_back(temp1);
        else
            flag=0;
    }
    return clusters1;
}
void normalize(cluster& cl){
    vector<data_point<double>> temp=cl.get_items();
    vector<double> rec(100,0.0);
    for(int i=0;i<cl.get_items().size();i++){
        for(int j=0;j<100;j++){
            rec[j]+=temp[i].point[j];
        }
    }
    for(int j=0;j<100;j++){
        rec[j]/=cl.get_items().size();
    }
    data_point<double> new_centroid;
    new_centroid.point=rec;
    cl.set_centroid(new_centroid);
    return;
}



void cros_val( map<int,data_point<double>> feels,vector<string> *coinz){
    int part_size=(feels.size()/10)+1;
    int start=0;
    int end=part_size;
    int table_size,j=0,users_feels=feels.size();
    map<int,data_point<double>>::iterator fit;
    for(int i=0;i<10;i++) {
        //vector<data_point<double>> test = get_part(feels, i);
        //erase_rand_coin(test);

        vector<int> r;
        vector<Hash_table> tables ;
        data_point<double> data_set[users_feels];
        j=0;
        for (fit=feels.begin();fit!=feels.end();fit++){
            data_set[j]=fit->second;
            j++;
        }
        random_vector(r,const_lsh::k);
        table_size=create_tables(tables,"cosine",users_feels,100);
        feed_tables(tables,data_set,table_size,users_feels,r);

        map<string, value_point<double>> bucks;
        vector<recom> recomm;
        Key query_key;
        for (int k = 0; k < feels.size(); k++) {
            for (int i = 0; i < tables.size(); i++) {
                query_key = tables[i].query_item(data_set[k], table_size, r);
                tables[i].get_bucket(data_set[k], query_key, bucks, r);
            }
            if (bucks.size() == 0) continue;
            recomm = rec_nn(bucks, data_set[k]);
            for(int z=0;z<100;z++){
                if (recomm[z].value);
            }
        }
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

    /* Cosine LSH Recommendation */
    //5 best Coins
    dim=100;
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
    //cros_val(feels,coinz);
    tables.clear();
    feels.clear();
    cluster_feel(raw_tweets,feels,coinz,voc);
    int cluster_feels=feels.size();
    // Cosine LSH Recommendation
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
    feels.clear();

    //Clustering users
    //Give 5 Recom+mendations
    user_feels( raw_tweets, feels,coinz, voc);
    users_feels = feels.size();
    vector<cluster> clusters;
    clusters=create_kmeans_centroids(data_set,100,users_feels,"euclidean");
    assign_to_clusters(data_set,clusters,users_feels,"euclidean");
    vector<cluster> temp_v;
    int same=0;
    for(int rr=0;rr<100;rr++){
        for (int i=0;i<clusters.size(); i++){
            temp_v.push_back(clusters[i]);
        }
        for (int i=0;i<clusters.size();i++){
            vector<double> new_centrer;
            new_centrer = calculate_mean_centroid(clusters[i]);
            data_point<double> temp1;
            temp1.name="Mean";
            temp1.point=new_centrer;
            clusters[i].set_centroid(temp1);
            new_centrer.clear();
            temp1.point.clear();
        }
        for(int r=0;r<clusters.size();r++){
            clusters[r].empty_clitems();
        }
        assign_to_clusters(data_set,clusters,users_feels,"euclidean");
        for(int i=0;i<clusters.size();i++){
            if (clusters[i].check_equal(temp_v[i]) == 1 ){
                same++;
            }
        }
        if (same==clusters.size()){
            cout << "Cluster did not change. Exit rep: "<< rr << endl;
            temp_v.clear();
            break;
        }
        same=0;
        temp_v.clear();
        cout << "iterration: " << rr <<endl;
    }
    vector<data_point<double>> items;
    for (int i=0; i <clusters.size();i++) {
        normalize(clusters[i]);
        items = clusters[i].get_items();
        for (int j = 0; j < items.size(); j++) {
            recomm = rec_nn_cluster(clusters[i].get_centroid(), items[j]);
            sort(recomm.begin(),recomm.end(),myfunction);
            print_recom(recomm,items[j],5,coinz);
        }
    }
    clusters.clear();
    feels.clear();
    // Cosine LSH Recommendation
    //2 best Coins from clusters
    cluster_feel(raw_tweets,feels,coinz,voc);
    cluster_feels=feels.size();

    clusters=create_kmeans_centroids(data_set2,25,cluster_feels,"euclidean");
    assign_to_clusters(data_set2,clusters,cluster_feels,"euclidean");
    assign_to_clusters(data_set,clusters,cluster_feels,"euclidean");
    temp_v.clear();
    same=0;
    for(int rr=0;rr<100;rr++){
        for (int i=0;i<clusters.size(); i++){
            temp_v.push_back(clusters[i]);
        }
        for (int i=0;i<clusters.size();i++){
            vector<double> new_centrer;
            new_centrer = calculate_mean_centroid(clusters[i]);
            data_point<double> temp1;
            temp1.name="Mean";
            temp1.point=new_centrer;
            clusters[i].set_centroid(temp1);
            new_centrer.clear();
            temp1.point.clear();
        }
        for(int r=0;r<clusters.size();r++){
            clusters[r].empty_clitems();
        }
        assign_to_clusters(data_set,clusters,users_feels,"euclidean");
        for(int i=0;i<clusters.size();i++){
            if (clusters[i].check_equal(temp_v[i]) == 1 ){
                same++;
            }
        }
        if (same==clusters.size()){
            cout << "Cluster did not change. Exit rep: "<< rr << endl;
            temp_v.clear();
            break;
        }
        same=0;
        temp_v.clear();
        cout << "iterration: " << rr <<endl;
    }
    recomm.clear();
    items.clear();
    for (int i=0; i <clusters.size();i++) {
        normalize(clusters[i]);
        items = clusters[i].get_items();
        for (int j = 0; j < items.size(); j++) {
            recomm = rec_nn_cluster(clusters[i].get_centroid(), items[j]);
            sort(recomm.begin(),recomm.end(),myfunction);
            print_recom(recomm,items[j],2,coinz);
        }
    }
    return 0;
}


