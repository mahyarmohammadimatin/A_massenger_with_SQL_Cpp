#include <bits/stdc++.h>
#include <iostream>
using namespace std;
string myusername,myuserid;

void show_tweets(vector<string>);

void show_this_tweet(string id);string makespace(string);


void add_comment(vector<string>);


void like_tweet(vector<string>);

vector<string> database(string s);

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

string getmetime();
void checknewmassage();
void sendmassage(string sender,string getter,string massage);
bool findstr(string s,string target);

class signin{
public:
    bool registering();
    bool login();

    bool inaccount=false;

};

class usermenu{
public:
    void addtweet();
    void view_tweet();
    void setting();
    void massage();
};

class viewoptions{
public:
    void all();
    void name(bool);
    void like();
    void search();
};

int main(){
    int WhatToDo;
    signin sign;//have sign functions.
    usermenu user;//have user menu when user logged in.

    while(true){
        if(sign.inaccount){
            checknewmassage();
            cout<<"1.write new tweet\n"
                <<"2.search in tweets\n"
                <<"3.show my massages\n"
                <<"4.setting\n"
                <<"5.sign out my account\n"
                <<"? ";
            cin>>WhatToDo;cout<<endl;

            switch(WhatToDo){
            case 1:user.addtweet();break;
            case 2:user.view_tweet();break;
            case 3:user.massage();break;
            case 4:user.setting();break;
            case 5:sign.inaccount=false;break;
            }
        }

        else{
            cout<<"1.register\n"
                <<"2.login\n"
                <<"3.close program\n"
                <<"? ";
            cin>>WhatToDo;cout<<endl;

            switch(WhatToDo){

            case 1:sign.inaccount=sign.registering();
            if(sign.inaccount) cout<<"\nwelcome to tweeter "<<myusername<<"!\n\n";break;

            case 2:sign.inaccount=sign.login();
            if(sign.inaccount) cout<<"\nwelcome to tweeter "<<myusername<<"!\n\n";break;

            case 3:return 0;
            }
        }
    }
}

void checknewmassage(){
    vector<string> v=database("~$ SELECT FROM users WHERE username=="+myusername+";");
    v=spliter(v[0],' ');
    if(v[9]!="0"){
        string order="~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("
           +v[0]+','+v[1]+','+v[2]+','+v[3]+','+v[4]+','+v[5]+','+v[6]+','+v[7]+','+v[8]+",0,"+v[10]+");";
        database(order);
        v=spliter(v[9],'-');
        cout<<"your followings post new tweets:\n";
        for(int i=1;i<v.size();i++){
            show_this_tweet(v[i]);
        }
    }
    cout<<endl<<endl;
    v=database("~$ SELECT FROM users WHERE username=="+myusername+";");
    v=spliter(v[0],' ');
    v=spliter(v[10],'-');
    if(v.size()>1){
        cout<<"you have "<<v.size()-1<<" new massage.check inbox.\n\n";
    }
}

//class signin.
bool signin :: registering(){
    string name,username,password,password2;
    cout<<"Enter name:";cin>>name;
    cout<<"Enter username:";cin>>username;
    cout<<"Enter password:";cin>>password;
    cout<<"Enter password again:";cin>>password2;

    //check password.
    if(password!=password2){
        cout<<"wrong password. try again.";
        return registering();
    }

    //get last id to generate new id for our user.
    string order="~$ SELECT FROM users;";
    vector<string> v=database(order);
    for(int i=0;i<v.size();i++){
        if(spliter(v[i],' ')[2]==username) {cout<<"this username is already taken.\n";return false;}
    }
    int id=1;
    if(!v.empty()) if(v[0]!="") id =inter(spliter(v[v.size()-2],' ')[0])+1;

    //add new user infos to database.
    order="~$ INSERT INTO users VALUES ("+str(id)+','+name+','+username+','+password+','+getmetime()+",0,0,0,0,0,0);";
    database(order);

    //give value to our global variables.
    myusername=username;
    myuserid=str(id);

    return true;
}
bool signin :: login(){
    string username,password;
    cout<<"Enter username:";cin>>username;
    cout<<"Enter password:";cin>>password;

    //check password.
    string order="~$ SELECT FROM users WHERE username=="+username+";";
    vector<string> v=database(order);
    if(v.empty()) {cout<<"not found any username match "+username+".\n";return false;}
    string s=v[0];
    if(spliter(s,' ')[3]!=password) {cout<<"you write wrong password. try again.\n";return false;}

    //give value to our global variables.
    myusername=username;
    myuserid=spliter(s,' ')[0];
    return true;
}

//class usermenu.
void usermenu :: addtweet(){
    //get tweet.
    cout<<"write your tweet:\n";
    string s,order;
    cin>>s;
    if(s.size()>1000){cout<<"your tweet is too long.\n";return;}

    //find last id and generate new id for our tweet.
    order="~$ SELECT FROM tweets;";
    vector<string> v=database(order);
    int id=1;
    if(!v.empty()) if(v[0]!="") id=inter(spliter(v[v.size()-2],' ')[0])+1;

    //now we have id we can insert new tweet.
    order="~$ INSERT INTO tweets VALUES ("+ str(id) +','+ myusername + ",0,0,0,"+getmetime()+',' + s +");";
    if(!database(order).empty()) cout<<"something wrong\n";

    //update user.txt add tweet id to user's list of tweets.
    v=database("~$ SELECT FROM users WHERE username=="+myusername+";");
    v=spliter(v[0],' ');
    order="~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("
           +v[0]+','+v[1]+','+v[2]+','+v[3]+','+v[4]+','+v[5]+','+v[6]+"-"+str(id)+','+v[7]+','+v[8]+','+v[9]+','+v[10]+");";
    database(order);

    //tell followers that have new tweet
    vector<string> followers=spliter(v[8],'-');
    for(int i=1;i<followers.size();i++){
        v=database("~$ SELECT FROM users WHERE id=="+followers[i]+";");
        v=spliter(v[0],' ');

        //order="~$ UPDATE FROM users WHERE id=="+followers[i]+" VALUES ("
          // +v[0]+','+v[1]+','+v[2]+','+v[3]+','+v[4]+','+v[5]+','+v[6]+"-"+str(id)+','+v[7]+','+v[8]+','+v[9]+'-'+str(id)+','+v[10]+");";

        order="~$ UPDATE FROM users WHERE id=="+followers[i]+" VALUES ("
           +v[0]+','+v[1]+','+v[2]+','+v[3]+','+v[4]+','+v[5]+','+v[6]+','+v[7]+','+v[8]+','+v[9]+'-'+str(id)+','+v[10]+");";
        //cout<<order<<endl;
        database(order);
    }
    cout<<"\nyour tweet submitted successfully!\n\n";
}
void usermenu :: view_tweet(){
    int WhatToDo;
    cout<<"1.view all tweets\n"
        <<"2.view by name\n"
        <<"3.view liked tweets\n"
        <<"4.view my tweets\n"
        <<"5.search in tweets\n"
        <<"6.back to menu\n"
        <<"? ";cin>>WhatToDo;

    viewoptions view;

    switch(WhatToDo){
    case 1:view.all();break;
    case 2:view.name(false);break;
    case 3:view.like();break;
    case 4:view.name(true);break;
    case 5:view.search();break;
    case 6:return;
    }

    cout<<"now choose a tweet id for comment,like & more details(or -1 to back):";
    string id;cin>>id;
    cout<<endl<<endl;

    if(id=="-1"){
        this->view_tweet();
        return;
    }

    show_this_tweet(id);
}
void usermenu :: setting(){
    cout<<endl;
    int WhatToDo;
    string order="~$ SELECT FROM users WHERE username=="+myusername+';';
    vector<string> userinfo=spliter(database(order)[0],' ');
    cout<<"1.manage following\n"
        <<"2.manage followers\n"
        <<"3.change name\n"
        <<"4.change password\n"
        <<"5.back to menu\n"
        <<"? ";cin>>WhatToDo;
    switch(WhatToDo){

        case 1:{
            vector<string> following=spliter(userinfo[7],'-');
            if(following.size()==1) cout<<"you aren't following anybody.\n";
            else{
                vector<string> v;
                cout<<"you following:\n";
                for(int i=1;i<following.size();i++){
                    v=spliter(database("~$ SELECT FROM users WHERE id=="+following[i]+';')[0],' ');
                    cout<<v[1]<<"(id: "+v[0]+")"<<endl;
                }
                cout<<"write id to unfollow or -1 to back to setting:";
                string a;cin>>a;
                if(a!="-1"){
                    int i=0;
                    string s=userinfo[7];
                    while(i<s.size()){
                        if(s[i]==a[0]){
                            break;
                        }
                        //new
                        i++;
                    }
                    s=s.substr(0,i-1) + s.substr(i+1,s.size()-i-1);
                    //new
                    cout<<s<<endl;
                    //database("~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("+
                             //userinfo[0]+','+userinfo[1]+','+userinfo[2]+','+userinfo[3]+','+userinfo[4]+','+userinfo[5]+','+userinfo[6]+','+s+','+userinfo[8]+','+userinfo[9]+','+userinfo[10]);

                    database("~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("+
                             userinfo[0]+','+userinfo[1]+','+userinfo[2]+','+userinfo[3]+','+userinfo[4]+','+userinfo[5]+','+userinfo[6]+','+s+','+userinfo[8]+','+userinfo[9]+','+userinfo[10]+");");
                    cout<<"you unfollow "+v[1]+".\n\n";
                }
            }
            setting();
            break;
        }

        case 2:{
            vector<string> followers=spliter(userinfo[8],'-');
            if(followers.size()==1) cout<<"nobody following you.\n";
            else{
                vector<string> v;
                cout<<"your followers:\n";
                for(int i=1;i<followers.size();i++){
                    v=spliter(database("~$ SELECT FROM users WHERE id=="+followers[i]+';')[0],' ');
                    cout<<v[1]<<"(id: "+v[0]+")"<<endl;
                }
                cout<<"write id to delete follower -1 to back to setting:";
                string a;cin>>a;
                if(a!="-1"){
                    int i=0;
                    string s=userinfo[8];
                    while(i<s.size()){
                        if(s[i]==a[0]){
                            break;
                        }
                        //new;
                        i++;
                    }
                    s=s.substr(0,i-1) + s.substr(i+1,s.size()-i-1);
                    database("~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("+
                             userinfo[0]+','+userinfo[1]+','+userinfo[2]+','+userinfo[3]+','+userinfo[4]+','+userinfo[5]+','+userinfo[6]+','+userinfo[7]+','+s+','+userinfo[9]+','+userinfo[10]);
                    cout<<"you delete "+v[1]+"from your followers.\n\n";
                }
            }
            setting();
            break;
        }

        case 3:{
            cout<<"Enter your new name:";
            string s;cin>>s;
            database("~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("+
                             userinfo[0]+','+s+','+userinfo[2]+','+userinfo[3]+','+userinfo[4]+','+userinfo[5]+','+userinfo[6]+','+userinfo[7]+','+userinfo[8]+','+userinfo[9]+','+userinfo[10]+");");
            setting();
            break;
        }

        case 4:{
            cout<<"Enter your new password:";
            string s;cin>>s;
            database("~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("+
                             userinfo[0]+','+userinfo[1]+','+userinfo[2]+','+s+','+userinfo[4]+','+userinfo[5]+','+userinfo[6]+','+userinfo[7]+','+userinfo[8]+','+userinfo[9]+','+userinfo[10]+");");
            setting();
            break;
        }

        case 5:return;
    }
}
void usermenu :: massage(){
    cout<<endl;
    string order="~$ SELECT FROM users WHERE username=="+myusername+';';
    vector<string> userinfo=spliter(database(order)[0],' ');
    vector<string> v=spliter(userinfo[10],'-');
    if(v.size()==1) cout<<"you don't have any massage.\n";
    else{
        cout<<"here is your massages:\n";
        vector<string> v2;
        for(int i=1;i<v.size();i++){
            v2=spliter(database("~$ SELECT FROM massages WHERE id=="+v[i]+';')[0],' ');
            //cout<<"send from "<<spliter(database("~$ SELECT FROM users WHERE id=="+v2[1]+';')[0],' ')[1]<<"(id: "+v2[1]+")"<<" at "<<v2[3]<<":\n"<<v2[4]<<"\n\n";
            cout<<"send from "<<spliter(database("~$ SELECT FROM users WHERE id=="+v2[1]+';')[0],' ')[1]<<"(id: "+v2[1]+")"<<" at "<<v2[3]<<":\n"<<v2[5]<<"\n\n";
        }
        cout<<endl<<"Enter user id to reply to the massage or -1 to back to menu:";
        string id;cin>>id;
        if(id=="-1") return;
        cout<<"Enter your massage:";string s;cin>>s;
        sendmassage(userinfo[0],id,s);
    }
}


//class viewoptions.
void viewoptions :: all(){
    cout<<"here is all tweets:\n";
    string order;
    order="~$ SELECT FROM tweets;";
    vector<string> v=database(order);
    show_tweets(v);
}
void viewoptions :: name(bool me){
    string order,s;
    if(!me){
        //new
        cout<<"Enter a name:";cin>>s;
        cout<<"Here is "+s+"'s tweets:\n";
        //string username=spliter(database("-$ SELECT FROM users WHERE name=="+s+";")[0],' ')[2];
        order="~$ SELECT FROM tweets WHERE person_username=="+s+";";

        vector<string> v=database(order);
        if(v.empty()) cout<<"not found any tweet with name "<<s<<".\n";
        show_tweets(v);
    }
    else{
        order="~$ SELECT FROM tweets WHERE person_username=="+myusername+";";
        vector<string> v=database(order);
        if(v.empty()) cout<<"you don't have any tweet yet :(\n";
        cout<<"here is your tweets:\n";
        show_tweets(v);
    }
}
void viewoptions :: like(){
    cout<<"you liked this tweets:\n";
    string order;
    order="~$ SELECT FROM users WHERE username==" + myusername +";";
    vector<string> v=spliter(spliter(database(order)[0],' ')[5],'-');
    vector<string> result;
    for(int i=1;i<v.size();i++){
        result.push_back(database("~$ SELECT FROM tweets WHERE tweet_id=="+v[i]+";")[0]);
    }
    show_tweets(result);
}
void viewoptions :: search(){
    cout<<"Enter a word to search:";string s;cin>>s;
    vector<string> tweets=database("~$ SELECT FROM tweets;");
    vector<string> result;
    for(int i=0;i<tweets.size()-1;i++){
        if(findstr(tweets[i],s)) result.push_back(tweets[i]);
    }
    show_tweets(result);
}

void show_tweets(vector<string> v){
    vector<string> line;
    for(int i=0;i<v.size();i++){
        if(v[i]=="") continue;
        line=spliter(v[i],' ');
        string name=spliter(database("~$ SELECT FROM users WHERE username=="+line[1]+";")[0],' ')[1];
        cout<<line[0]<<" "<<line[6].substr(0,20)<<"...(by: "<<name<<")"<<endl;
    }
    cout<<endl<<endl;
}

void show_this_tweet(string id){
    string order="~$ SELECT FROM tweets WHERE tweet_id=="+id+";";
    vector<string> tweet=spliter(database(order)[0],' ');
    cout<<tweet[0]<<" "<<makespace(tweet[6])<<endl<<endl;
    cout<<"written by "<<tweet[1]<<" in "<<tweet[5];
    if(tweet[2]!="0") cout<<"(retweet of id:"<<tweet[2]<<")";
    cout<<endl<<endl<<"comments:\n";
    vector<string> v=spliter(tweet[3],'-');
    if(v.size()==1) cout<<"not found any comment :(\n";
    else{
        vector<string> comment;
        for(int i=1;i<v.size();i++){
            comment=spliter(database("~$ SELECT FROM comments WHERE id=="+v[i]+";")[0],' ');
            cout<<comment[0]<<" "<<comment[2]<<":"<<comment[4]<<endl;
        }
        cout<<endl<<endl;
    }
    cout<<endl<<endl;
    while(true){
    int WhatToDo;
    cout<<"1.like this tweet\n"
        <<"2.write comment on this tweet\n"
        <<"3.show who's like this tweet\n"
        <<"4.retweet this tweet\n"
        <<"5.follow writer\n"
        //new
        <<"6.massage to writer\n"
        //new
        <<"7.back to menu\n"
        <<"? ";cin>>WhatToDo;cout<<endl;
    switch(WhatToDo){
    case 1:{
        like_tweet(tweet);
        break;
    }

    case 2:
    {
        add_comment(tweet);
        break;
    }
    case 3:
    {
        cout<<"liked by:";
        v=spliter(tweet[4],'-');
        vector<string> userinfo;
        if(v.size()==1){
            cout<<"no one like this tweet yet.\n";
            break;
        }
        for(int i=1;i<v.size();i++){
            userinfo=spliter(database("~$ SELECT FROM users WHERE id=="+v[i]+';')[0],' ');
            cout<<userinfo[1];
            if(i!=v.size()-1) cout<<',';
        }
        cout<<'.';
        break;
    }

    case 4:
    {
        string order="~$ SELECT FROM tweets;";
        vector<string> v=database(order);
        int id=inter(spliter(v[v.size()-2],' ')[0])+1;
        order="~$ INSERT INTO tweets VALUES ("+ str(id) +','+ myusername + ","+tweet[0]+",0,0,"+ getmetime() + "," + tweet[6] +");";
        if(!database(order).empty()) cout<<"something wrong\n";

        v=database("~$ SELECT FROM users WHERE username=="+myusername+";");
        v=spliter(v[0],' ');
        order="~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("
               +v[0]+','+v[1]+','+v[2]+','+v[3]+','+v[4]+','+v[5]+','+v[6]+"-"+str(id)+','+v[7]+','+v[8]+','+v[9]+','+v[10]+");";
        database(order);
        cout<<"\nretweeted successfully!\n";
        break;
    }

    case 5:{
        vector<string> v;
        //string id=spliter(database("~$ SELECT FROM users WHERE username=="+tweet[1]+";")[0],' ')[0];
        vector<string> v2=spliter(database("~$ SELECT FROM users WHERE username=="+tweet[1]+";")[0],' ');

        if(id!=myuserid){
            v=spliter(database("~$ SELECT FROM users WHERE username=="+myusername+";")[0],' ');
            database("~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("+
                     v[0]+','+v[1]+','+v[2]+','+v[3]+','+v[4]+','+v[5]+','+v[6]+','+v[7]+'-'+v2[0]+','+v[8]+','+v[9]+','+v[10]+");");

            //new
            database("~$ UPDATE FROM users WHERE id=="+id+" VALUES ("+v2[0]+','+
                     v2[1]+','+v2[2]+','+v2[3]+','+v2[4]+','+v2[5]+','+v2[6]+','+v2[7]+','+v2[8]+'-'+myuserid+','+v2[9]+','+v2[10]+");");
            //new
        }
        else{
            cout<<"you can't follow yourself :|";
        }
        break;
    }

    case 6:{
        cout<<"write your massage:";string s;cin>>s;
        sendmassage(myuserid,spliter(database("~$ SELECT FROM users WHERE username=="+tweet[1]+";")[0],' ')[0],s);
    }

    case 7:return;
    }
    cout<<endl<<endl;
    }
}
string makespace(string s){
    //for(int i=0;i<s.size();i++) if(s[i]=='_') s[i]==' ';
    string result="";
    for(int i=0;i<s.size();i++) if(s[i]=='_') result+=' ';
    else result+=s[i];
    return result;
}


void like_tweet(vector<string> tweet){
    vector<string> userinfo=spliter(database("~$ SELECT FROM users WHERE username=="+myusername+';')[0],' ');
    string order;
    vector<string> v=spliter(tweet[4],'-');
    for(int i=0;i<v.size();i++){
        if(v[i]==myuserid){
            cout<<"you already like this tweet. try another one.\n";
            return;
        }
    }
    order="~$ UPDATE FROM tweets WHERE tweet_id=="+tweet[0]+" VALUES ("+
               tweet[0]+',' + tweet[1]+',' + tweet[2]+',' + tweet[3]+',' + tweet[4]+"-"+myuserid+',' + tweet[5]+',' + tweet[6] +");";

    database(order);


    order="~$ UPDATE FROM users WHERE username=="+myusername+" VALUES ("+
            userinfo[0]+','+userinfo[1]+','+userinfo[2]+','+userinfo[3]+','+userinfo[4]+','+userinfo[5]+"-"+tweet[0]+','+userinfo[6]+','+userinfo[7]+','+userinfo[8]+','+userinfo[9]+','+userinfo[10]+");";

    database(order);

    cout<<"tweet liked successfully.\n";
}
void add_comment(vector<string> tweet){
    cout<<"write your comment:\n";
    string s;cin>>s;
    vector<string> v=database("~$ SELECT FROM comments;");
    string id="1";
    if(!v.empty()) if(v[0]!="") id=str(inter(spliter(v[v.size()-2],' ')[0])+1);
    string order="~$ INSERT INTO comments VALUES ("+id+','+tweet[0]+','+myusername+','+getmetime()+','+s+");";
    database(order);
    order="~$ UPDATE FROM tweets WHERE tweet_id=="+tweet[0]+
          " VALUES ("+tweet[0]+','+tweet[1]+','+tweet[2]+',' +tweet[3]+'-'+id +','+tweet[4]+','+tweet[5]+','+tweet[6]+");";
    v=database(order);
    if(!v.empty()) cout<<v[0]<<endl;
    cout<<"\nyour comment submited successfully.\n";
}


string getmetime(){
    time_t now = time(0);
    vector<string> v=spliter(ctime(&now),' ');
    return v[4].substr(0,4)+'/'+v[1]+'/'+v[2]+"--at:"+v[3];
}
bool findstr(string s,string target){
    bool flg;
    for(int i=0;i<s.size();i++){
        if(s[i]==target[0]){
            flg=true;
            for(int j=0;j<target.size();j++){
                if(s[i+j]!=target[j]) flg=false;
            }
            if(flg){
                return true;
            }
        }
    }
    return false;
}
void sendmassage(string sender,string getter,string massage){
    vector<string> v=database("~$ SELECT FROM massages;");
    string id="1";
    if(!v.empty()) if(v[0]!="") id=str(inter(spliter(v[v.size()-2],' ')[0])+1);
    database("~$ INSERT INTO massages VALUES ("+id+','+sender+','+getter+','+getmetime()+",false,"+massage+");");
    v=database("~$ SELECT FROM users WHERE id=="+getter+";");
    v=spliter(v[0],' ');
    //new
    database("~$ UPDATE FROM users WHERE id=="+getter+" VALUES ("+v[0]+','+v[1]+','+v[2]+','+v[3]+','+v[4]+','+v[5]+','+v[6]+','+v[7]+','+v[8]+','+v[9]+','+v[10]+'-'+id+");");
}


//database


vector<string> database(string s){
    vector<string> result;
    if(s.substr(0,3)=="~$ "){
        if(s.substr(3,12)=="INSERT INTO ") result=inserter(s.substr(15,s.size()-15));
        else if(s.substr(3,12)=="SELECT FROM ") result=selecter(s.substr(15,s.size()-15));
        else if(s.substr(3,12)=="DELETE FROM ") result=deleter(s.substr(15,s.size()-15));
        else if(s.substr(3,12)=="UPDATE FROM ") result=updater(s.substr(15,s.size()-15));
        else result.push_back("SYNTAX ERROR:\nthe order is wrong.\n");
    }
    else result.push_back("SYNTAX ERROR:\nat the first of any order should be -$\n");
    return result;
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
        getline(schema,s);
        if(s==tablename){flg=true;break;}
    }
    if(!flg) {cout<<"DATABASE ERROR:\nnot found any table with name "+tablename+".\n";return false;}
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
                getline(datafile,restr);
                vector<string> v;
                v=spliter(restr,' ');
                if(v[i]==values[i]) {cout<<"DATABASE ERROR:\nthe value that you enter is unique but we already have it.";return false;}
            }
            datafile.close();
        }
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
            while(!myfile.eof()){
                getline(myfile,s);
                if(spliter(s,' ')[which-1]==value) finalresult.push_back(s);
            }
        }
        if(c=='!'){
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
    getline(myfile,s);
    while(!myfile.eof()){
        for(int i=0;i<wanted_lines.size();i++){
            if(wanted_lines[i]==s) flg=false;
        }
        if(flg) allfile.push_back(s);
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


