#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

void bfs(int stnum,int ennum,int sizea,int error, vector<vector<int> > & array0){
    int v;
    int tmp;
    queue<int> q;
    bool tof[sizea+1];
    int path[sizea+1];
    vector <int> shortpath;
    q.push(stnum);
    path[stnum]=0;
    path[0]=0;
    for(int boo=0;boo<sizeof(tof);boo++){
        tof[boo]=false;
    }
    tof[stnum]= true;
    while (q.empty()==0){
        for (int u=0;u<array0[q.front()].size();u++){
            int tempN = array0[q.front()][u];
            if (tof[tempN]==false){
                tof[tempN]=true;
                q.push(tempN);
                path[tempN]=q.front();   
            }  
        }
        q.pop();
    }
    if (tof[ennum]==true){
        v=ennum;
        while (v!=stnum){
            shortpath.push_back(v);
            tmp=v;
            v=path[tmp];
        }
        reverse(shortpath.begin(),shortpath.end());
        cout<<stnum;
        for (int m=0;m<shortpath.size();m++){
            cout<<"-"<<shortpath[m];
        }
        cout<<endl;
    }
    else {
        cout<<"Error: there is no shortest path from "<<stnum<<" to "<<ennum<<endl;
        error=1;
    }
}
string n;
int main(){
    int a;
    int errorvertex=0;
    int errorpath=0;
    vector<vector<int> > array1;
    while(getline(cin,n)){
        if (n[0]=='V')
        {   
            errorvertex=0;
            a=0;
            for (int i=2;i<n.length();i++){
                a=n[i]-48+a*10;
            }
            if (a==0){
                cout<<"Error:there is no vertex"<<endl;
                errorvertex=1;
            }
            array1 = vector<vector<int> >(a+1);
        }
        if (n[0]=='E'){
            int len=n.length();
            string lnumber;
            string rnumber;
            int left=0;
            int right=0;
            for (int j=0 ;j<len;j++) {
                if (n[j]=='}'){
                    break;
                }
                else {
                    if (n[j]=='<'){
                        j+=1;
                        lnumber="";
                        left=0;
                        for (;n[j]!=',';j++){
                        lnumber+=n[j];
                    }
                    if (n[j]==',' and n[j+1]!='<'){
                        j+=1;
                        rnumber="";
                        right=0;
                        for (;n[j]!='>';j++){
                            rnumber+=n[j];
                        }
                    }
                    left=stoi(lnumber);
                    right=stoi(rnumber);
                    if (left>a){
                        cout<<"Error:vertex "<<left<<" does not exist"<<endl;
                        errorvertex=1;
                        break;
                    }
                    else if (right>a){
                        cout<<"Error:vertex "<<right<<" does not exist"<<endl;
                        errorvertex=1;
                        break;
                    }
                    else{
                        array1[left].push_back(right);
                        array1[right].push_back(left);    
                    }
                    }
                    }

                }
        }
        if (n[0]=='s'){
            errorpath=0;
            int len=n.length();
            int start=0;
            int end=0;
            int count0=0;
            string str0="";
            for (int z=1;z<len && count0<2;z++){
                if (n[z]!=' '){
                    str0 +=n[z];
                }
                if (count0==0 && str0!="" && n[z+1]==' '){
                    start=stoi(str0);
                    count0++;
                    str0="";
                }
                else if (count0==1 && str0!="" && (n[z+1]==' '|| z==len-1)){
                    end=stoi(str0);
                    count0++;
                    str0="";
                }
            }
            if (start>a){
                cout<<"Error:vertex "<<start<<" does not exist"<<endl;
                errorpath=1;
                continue;
            }
            if (end>a){
                cout<<"Error:vertex "<<end<<" does not exist"<<endl;
                errorpath=1;
                continue;
            }
            if (array1[start].size()==0 || array1[end].size()==0){
                cout<<"Error:there is no shortest path from "<<start<<" to "<<end<<endl;
                errorpath=1;
                continue;
            }
            if(errorvertex==0 && errorpath==0){
                bfs(start, end, a, errorpath, array1);
            } 
            if(errorvertex==1){
                cout<<"Error:wrong inputs"<<endl;
            }
        }
    }
    return 0;
}