#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
using namespace std;

vector<string> inserter(string);
bool errorhandeler(string tablename,vector<string> values,bool);
vector<string> spliter(string,char);
bool check_type(string,string);
int inter(string);string str(int);

vector<string> selecter(string order);
vector<string> whichlines(string s,string tablename);
string dlt_outer_parantes(string s);
int find_outer_oprt(string s);

vector<string> deleter(string order);
vector<string> updater(string order);


int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024]={0}; 
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
		{ 
			perror("listen"); 
			exit(EXIT_FAILURE); 
		} 
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
						(socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
	while(true){
		cout<<"server is runnig..."<<endl;
		read( new_socket , buffer, 1024);
		string s=buffer;
		cout<<s<<endl;
		vector<string> result;
    		if(s.substr(0,3)=="~$ "){
        	if(s.substr(3,12)=="INSERT INTO ") inserter(s.substr(15,s.size()-15));
        	else if(s.substr(3,12)=="SELECT FROM ") result=selecter(s.substr(15,s.size()-15));
        	else if(s.substr(3,12)=="DELETE FROM ") deleter(s.substr(15,s.size()-15));
        	else if(s.substr(3,12)=="UPDATE FROM ") updater(s.substr(15,s.size()-15));
        	else result.push_back("SYNTAX ERROR:\nthe order is wrong.\n");
    		}
    		else result.push_back("SYNTAX ERROR:\nat the first of any order should be -$\n");
		s="empty";
		if(!result.empty()) if(result[0]!=""){
			s="";
			for(int i=0;i<result.size();i++){
				s+=result[i]+'*';
			}
			s=s.substr(0,s.size()-1);
			 send(new_socket , s.c_str() , s.length() , 0 );
		}
		else send(new_socket , s.c_str() , s.length() , 0 );

	}
    return 0; 
} 


vector<string> inserter(string s){
    vector<string> result;
    string tablename="",value="";
    int i=0;
    while(s[i]!=' ') {tablename+=s[i];i++;}
    i++;
    while(s[i]!=' ') {value+=s[i];i++;}
    if(value!="VALUES") {result.push_back("SYNTAX ERROR:\nyou must type keyword VALUES\n");return result;}
    i++;
    s=s.substr(i,s.size()-i);
    if( s[0]!='(' || s[s.size()-2]!=')'){result.push_back("SYNTAX ERROR:\nyou miss values paranteses.\n");return result;}
    if(s[s.size()-1]!=';') {result.push_back("SYNTAX ERROR:\nyou miss ; at the end of order\n");return result;}
    vector<string> values=spliter(s.substr(1,s.size()-3),',');

    if(!errorhandeler(tablename,values,false)){
        result.push_back("DATABASE ERROR\n");
        return result;
    }

    ofstream datafile2((tablename+".txt").c_str(),ios::app);
    for(int j=0;j<values.size();j++){
        datafile2<<values[j];
        if(j!=values.size()-1)datafile2<<' ';
    }
    datafile2<<endl;
    datafile2.close();
    return result;
}
bool errorhandeler(string tablename,vector<string> values,bool isupdate){
    string s;
    ifstream schema("schema.txt");
    bool flg=false;
    while(!schema.eof()){
	schema.ignore();
        getline(schema,s);
	cout<<s<<s.size()<<endl;//herreeeee
        if(s==tablename){flg=true;break;}
    }
    if(!flg) {cout<<"DATABASE ERROR:\nnot found any table with name "+tablename+".\n";return false;}
	schema.ignore();
    getline(schema,s);int i=0;
    vector<string> field;
    ofstream makefile((tablename+".txt").c_str(),ios::app);
    makefile.close();
    string restr;
    while(s!=""){
        field=spliter(s,' ');
        if(i>=values.size()){cout<<"DATABASE ERROR:\nthe number of value you enter isn't enough.\n";return false;}
        if(field.size()==3){
            if(!check_type(values[i],field[2])) flg=false;
        }
        if(field.size()==2){
            if(!check_type(values[i],field[1])) flg=false;
        }
        if(!flg){cout<<"DATABASE ERROR:\ntype of "+str(i+1)+"'th value you enter is different from schema.\n";return false;}
        if(field.size()==3 && !isupdate){
            ifstream datafile((tablename+".txt").c_str());
            while(!datafile.eof()){
datafile.ignore();
                getline(datafile,restr);
                vector<string> v;
                v=spliter(restr,' ');
                if(v[i]==values[i]) {cout<<"DATABASE ERROR:\nthe value that you enter is unique but we already have it.";return false;}
            }
            datafile.close();
        }
schema.ignore();
        i++;getline(schema,s);
    }
    schema.close();
    if(i!=values.size()){cout<<"DATABASE ERROR:\nthe number of value you enter are so many.\n";return false;}
    return true;
}
vector<string> spliter(string s,char c){
    string str;
    vector<string> result;
    for(int i=0;i<s.size();i++){
        if(s[i]!=c) str+=s[i];
        else{
            result.push_back(str);
            str="";
        }
    }
    result.push_back(str);
    return result;
}
bool check_type(string a,string b){
    if(b=="TIMESTAMP"){
        return true;
    }
    if(b=="BOOLEAN"){
        if(a=="true" || a=="false") return true;
        return false;
    }
    if(b=="INTEGER"){
        bool flg=false;
        for(int i=0;i<a.size();i++){
            for(int j=0;j<10;j++){
                if(a[i]=="0123456789"[j]) flg=true;
            }
            if(!flg) return false;
            flg=false;
        }
        return true;
    }

    if(b.size()>7)if(b.substr(0,7)=="INTEGER"){
        vector<string> v;
        b=b.substr(8,b.size()-9);
        int limit=inter(b);
        v=spliter(a,'-');
        if(v.size()>limit) return false;
        for(int i=0;i<v.size();i++){
            if(!check_type(v[i],"INTEGER"))return false;
        }
        return true;
    }
    if(b.size()>4)if(b.substr(0,4)=="CHAR"){
        if(a.size()>inter(b.substr(5,b.size()-6))) return false;
        return true;
    }
    return false;
}
int inter(string s){
    string dict="0123456789";
    int re=0;
    for(int i=0;i<s.size();i++){
        for(int j=0;j<10;j++){
            if(s[i]==dict[j]){
                re=re*10+j;
                break;
            }
        }
    }
    return re;
}
string str(int a){
    if(a==0) return "0";
    string re="";
    string dict="0123456789";
    while(a!=0){
        re=dict[a%10] + re;
        a=a/10;
    }
    return re;
}



vector<string> selecter(string order){
    vector<string> result;
    string tablename="";
    string s;
    int i=0;
    while(order[i]!=' ' && order[i]!=';') {tablename+=order[i];i++;}

    ofstream myfile((tablename+".txt").c_str(),ios::app);
    myfile.close();

    if(order[i]==';'){
        ifstream myfile((tablename+".txt").c_str());
        while(!myfile.eof()){
myfile.ignore();
            getline(myfile,s);
            result.push_back(s);
        }
        myfile.close();
        return result;
    }
    i++;
    if(order.substr(i,5)!="WHERE") {result.push_back("SYNTAX ERROR:\nyou must type keyword WHERE.\n");return result;}
    if(order[order.size()-1]!=';') {result.push_back("SYNTAX ERROR:\nyou must type ; at the end.\n");return result;}
    order=order.substr(i+6,order.size()-i-7);

    return whichlines(order,tablename);
}

vector<string> whichlines(string s,string tablename){
    s=dlt_outer_parantes(s);
    vector<string> result1;
    vector<string> result2;
    int oprator=find_outer_oprt(s);
    if(oprator==-1){
        int i=0;
        while(i<s.size()){
            if(s[i]=='='){
                if(s[i+1]=='=')i++;
                break;
            }
            i++;
        }
        char c=s[i-1];
        string field_name=s.substr(0,i-1);
        string value=s.substr(i+1,s.size()-i-1);
        int which=1;
        ifstream schema("schema.txt");
        while(!schema.eof()){
schema.ignore();
            getline(schema,s);
            if(s==tablename){

                getline(schema,s);
                while(s!=""){
                    if(field_name==spliter(s,' ')[0]) break;

                    getline(schema,s);
                    which++;
                }
            }
        }
        schema.close();
        ifstream myfile((tablename+".txt").c_str());
        vector<string> finalresult;
        if(c=='='){
myfile.ignore();
            while(!myfile.eof()){
                getline(myfile,s);
                if(spliter(s,' ')[which-1]==value) finalresult.push_back(s);
            }
        }
        if(c=='!'){
	myfile.ignore();
            while(!myfile.eof()){

                getline(myfile,s);
                if(spliter(s,' ')[which-1]!=value) finalresult.push_back(s);
            }
        }
        myfile.close();
        return finalresult;
    }
    if(s[oprator]=='O'){
        result1=whichlines(s.substr(0,oprator),tablename);
        result2=whichlines(s.substr(oprator+2,s.size()-oprator-2),tablename);

        bool flg=true;
        for(int i=0;i<result2.size();i++){
            for(int j=0;j<result1.size();j++){
                if(result1[j]==result2[i]) flg=false;
            }
            if(flg) result1.push_back(result2[i]);
            flg=true;
        }
        return result1;
    }
    if(s[oprator]=='A'){
        result1=whichlines(s.substr(0,oprator),tablename);
        result2=whichlines(s.substr(oprator+3,s.size()-oprator-3),tablename);
        vector<string> result;
        bool flg=false;
        for(int i=0;i<result2.size();i++){
            for(int j=0;j<result1.size();j++){
                if(result2[i]==result1[j]) flg=true;
            }
            if(flg) result.push_back(result2[i]);
            flg=false;
        }
        return result;
    }
}
string dlt_outer_parantes(string s){
    if(s[0]!='(') return s;
    int c=0;
    for(int i=0;i<s.size();i++){
        if(s[i]=='(') c++;
        if(s[i]==')') c--;
        if(c==0 && i!=s.size()-1) return s;
    }
    return dlt_outer_parantes(s.substr(1,s.size()-2));
}
int find_outer_oprt(string s){
    int c=0;
    for(int i=0;i<s.size();i++){
        if(s[i]=='(') c++;
        if(s[i]==')') c--;
        if(s[i]=='O' && s[i+1]=='R' && c==0) return i;
        if(s[i]=='A' && s[i+1]=='N' && s[i+2]=='D' && c==0) return i;
    }
    return -1;
}



vector<string> deleter(string order){
    vector<string> result;
    string tablename="";
    string s;
    int i=0;
    while(order[i]!=' ') {tablename+=order[i];i++;}
    i++;
    if(order.substr(i,5)!="WHERE") {result.push_back("SYNTAX ERROR:\nyou must type keyword WHERE.\n");return result;}
    if(order[order.size()-1]!=';') {result.push_back("SYNTAX ERROR:\nyou must type ; at the end.\n");return result;}
    order=order.substr(i+6,order.size()-i-7);
    vector<string> wanted_lines;
    vector<string> allfile;
    wanted_lines=whichlines(order,tablename);
    bool flg=true;
    ifstream myfile((tablename+".txt").c_str());
myfile.ignore();
    getline(myfile,s);
    while(!myfile.eof()){
        for(int i=0;i<wanted_lines.size();i++){
            if(wanted_lines[i]==s) flg=false;
        }
        if(flg) allfile.push_back(s);
myfile.ignore();
        getline(myfile,s);
        flg=true;
    }
    myfile.close();
    ofstream newfile((tablename+".txt").c_str());
    for(int i=0;i<allfile.size();i++){
        newfile<<allfile[i]<<endl;
    }
    newfile.close();
    return result;
}



vector<string> updater(string order){

    vector<string> result;
    string tablename="";
    string s;
    int i=0;
    while(order[i]!=' ') {tablename+=order[i];i++;}
    i++;
    if(order.substr(i,5)!="WHERE") {result.push_back("SYNTAX ERROR:\nyou must type keyword WHERE.\n");return result;}
    if(order[order.size()-1]!=';') {result.push_back("SYNTAX ERROR:\nyou must type ; at the end.\n");return result;}
    i+=6;
    string where,myvalues;
    while(order[i]!=' ') {where+=order[i];i++;}
    i++;
    while(order[i]!=' ') {myvalues+=order[i];i++;}
    i++;
    if(myvalues!="VALUES") {result.push_back("SYNTAX ERROR:\nyou must type keyword VALUES.\n");return result;}
    vector<string> values=spliter(order.substr(i+1,order.size()-i-3),',');
    if(!errorhandeler(tablename,values,true)){
        result.push_back("DATABASE ERROR\n");
        return result;
    }
    vector<string> lines=whichlines(where,tablename);
    vector<string> allfile;
    ifstream myfile((tablename+".txt").c_str());
    while(!myfile.eof()){
myfile.ignore();
        getline(myfile,s);
        allfile.push_back(s);
    }
    myfile.close();
    bool flg=false;
    ofstream updatedfile((tablename+".txt").c_str());
    for(int i=0;i<allfile.size();i++){
        for(int j=0;j<lines.size();j++){
            if(allfile[i]==lines[j]) flg=true;
        }
        if(flg){
            for(int j=0;j<values.size();j++){
                updatedfile<<values[j];
                if(j!=values.size()-1) updatedfile<<' ';
            }
        }
        else updatedfile<<allfile[i];
        if(i!=allfile.size()-1)updatedfile<<endl;
        flg=false;
    }
    return result;
}

