//
// Created by kosti on 11/20/2018.
//
#include "utilities.h"

int num_columns(string line) {
    int words=0;
    char c;
    for(int i=0;i<line.size(); i++){
        if (line[i] == ','){
            words++;
        }
    }
    words++;
    return words;
}

void get_data_lengths(string input,int& lines, int& d) {
    ifstream inputfd;
    string line;
    inputfd.open(input);
    if (inputfd.is_open()){
        getline(inputfd,line);
        d = num_columns(line);
        lines++;
        while ( getline(inputfd,line) ){
            lines++;
        }
        inputfd.close();
    }else{
        cerr << "Unable to open file: " << input << endl;
    }
    inputfd.close();
}

void feed_data_set(string input, data_point<double> *pPoint,int d,int num_lines) {
    char cc[1024] ;
    double dd;
    string name;
    string line;
    ifstream inputfd;
    inputfd.open (input);
    inputfd.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    int z=0;
    while(z<num_lines){
        for( int ii=0; ii< d-1 ;ii++){
            getline(inputfd,line,',');
            dd=stod(line);
            pPoint[z].point.push_back(dd);
        }
        getline(inputfd,line);
        dd=stod(line);
        pPoint[z].point.push_back(dd);
        pPoint[z].name="item_"+to_string(z+1);
        z++;
    }
    inputfd.close();
}

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

bool vectors_eq(std::vector<double> v1, std::vector<double> v2){
    for (int i = 0 ; i<v1.size() ; i++){
        if(v1[i]!=v2[i]){
            return false;
        }
    }
    return true;
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



void cluster_feels(map<int,tweet> raw_tweets, map<int,data_point<double>>& feels,vector<string>* coinz, map<string,float> voc){
    data_point<double> temp;
    temp.sum=0;
    vector<double> tmp;

    for(int i=0;i<100;i++)
        temp.point.push_back(0.0);
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
}

void print_feels(map<int,data_point<double>> feels){
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
}
