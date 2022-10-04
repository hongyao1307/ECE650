// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

int cnf_sat(int n, int k, vector<vector<int> > array0){
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    vector<int> vertex_cover;
    Minisat::Lit array_nk[k][n];
    for (int i=0; i<k ; i++){
        for (int j=0; j<n; j++){
            array_nk[i][j]=Minisat::mkLit(solver->newVar());
        }
    }
    //part 1
    Minisat::vec<Minisat::Lit> temp_clause;
    for (int i=0; i<k; i++){
            //Minisat::vec<Minisat::Lit> temp_clause;
        for (int j=0; j<n; j++){
            temp_clause.push(array_nk[i][j]);
        }
        solver->addClause(temp_clause);
        temp_clause.clear();
    }
    //part 2
    for (int i=0; i<n; i++){
        for (int j=0 ; j<k-1; j++){
            for (int l=j+1; l<k; l++){
                //temp_clause[count].push(~array_nk[j][i]);
                //temp_clause[count].push(~array_nk[l][i]);
                //solver->addClause(temp_clause[count]);
                //count+=1;
                solver->addClause(~array_nk[j][i],~array_nk[l][i]);
            }
        }
    }
    //part 3
    for (int i=0; i<k; i++){
        for (int j=0; j< n-1; j++){
            for (int l=j+1; l<n; l++){
                //temp_clause[count].push(~array_nk[i][j]);
                 //temp_clause[count].push(~array_nk[i][l]);
                //solver->addClause(temp_clause[count]);
                //count+=1;
                solver->addClause(~array_nk[i][j],~array_nk[i][l]);
            }
        }
    }
    //part 4
    for (int i=0; i<array0.size(); i++){
        for (int j=0; j<array0[i].size();j++){
                //Minisat::vec<Minisat::Lit> temp_clause;
            if (array0[i][j]>i){
                for (int a=0; a<k; a++){
                    temp_clause.push(array_nk[a][i-1]);
                    temp_clause.push(array_nk[a][array0[i][j]-1]);
                }
                solver->addClause(temp_clause);
                temp_clause.clear();
                //count+=1;
            }
        }
    }
    //solver
    //cout<<"size:"<<count<<" "<<temp_clause.size()<<endl;
    //for (int i=0; i<temp_clause.size(); i++){
        //solver->addClause(temp_clause[i]);
    //}
    //solver->addClause(temp_clause);
    bool result= solver->solve();
    if (result){
        for (int i=0; i<k ; i++){
            for (int j=0; j<n; j++){
                if (Minisat::toInt(solver->modelValue(array_nk[i][j]))==0){
                    vertex_cover.push_back(j+1);
                }
            }
        }
        sort(vertex_cover.begin(),vertex_cover.end());
        for (int i=0; i<vertex_cover.size(); i++){
            if (i==vertex_cover.size()-1){
                cout<<vertex_cover[i]<<endl;
            }
            else{
                cout<<vertex_cover[i]<<" ";
            }
        }
        solver.reset (new Minisat::Solver());
        return 0;
    }
    else {
        solver.reset (new Minisat::Solver());
        return 1;
    }
}

string n;
int main(){
    int a;
      //int errorvertex=0;
      //int errorpath=0;
    vector<vector<int> > array1;
    while(getline(cin,n)){
        if (n[0]=='V')
        {

            a=0;
            for (int i=2;i<n.length();i++){
                a=n[i]-48+a*10;
            }
            if (a==0){
                cout<<"Error:there is no vertex"<<endl;

            }
            array1 = vector<vector<int> >(a+1);
        }
        if (n[0]=='E'){
            int len=n.length();
            string lnumber;
            string rnumber;
            int vertex=0;
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

                        break;
                    }
                    else if (right>a){
                        cout<<"Error:vertex "<<right<<" does not exist"<<endl;

                        break;
                    }
                    else{
                        array1[left].push_back(right);
                        array1[right].push_back(left);
                        }
                    }
                    }
            }
            for (int i=1; i<a+1; i++){
                if (cnf_sat(a, i, array1)==0){
                    break;
                }
                else if(cnf_sat(a, i, array1)==1) {
                    continue;
                }
            }
        }
    }
    return 0;
}

