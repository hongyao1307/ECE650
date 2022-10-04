#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "a3street.h"
#include "a3graph.h"
using namespace std;

int graph::same_points1(){
    if (x1==x2 && y1==y2){
        return 0;
    }
    else {
        return 1;
    }
}

int graph::same_points2(){
    if (x3==x4 && y3==y4){
        return 0;
    }
    else {
        return 1;
    }
}

int graph::if_vertical1(){
    if (x1==x2){
        return 0;
    }
    else {
        return 1;
    }
}

int graph::if_vertical2(){
    if (x3==x4){
        return 0;
    }
    else {
        return 1;
    }
}

void graph::xy_formula1(){
    k1=(y2-y1)/(x2-x1);
    b1=y1-k1*x1;
}

void graph::xy_formula2(){
    k2=(y4-y3)/(x4-x3);
    b2=y3-k2*x3;
}

int graph::vertical_overlapping(){
    if (y1<y3<y4||y1<y4<y3||y3<y1<y4||y3<y2<y4){
        return 0;
    }
    else {
        return 1;
    }
}

int graph::if_overlapping(){
    if (k1==k2 && b1==b2){
        ifk=0;
    }
    else{
        ifk=1;
    }
    if (ifk==0){
        if (x3<x1<x4||x3<x2<x4||x1<x3<x2||x1<x4<x2){
            return 0;
        }
        else {
            return 1;
        }
    }
    else{
        return 1;
    }
}

int graph::inter_sect(){
    if (max(x1,x2)<min(x3,x4) || max(y1,y2)<min(y3,y4) || max(x3,x4)<min(x1,x2) || max(y3,y4)<min(y1,y2)){
        return 1;
    }
    else if ((((x1 - x3)*(y4 - y3) - (y1 - y3)*(x4 - x3))*
        ((x2 - x3)*(y4 - y3) - (y2- y3)*(x4 - x3))) >0 ||
        (((x3 - x1)*(y2 - y1) - (y3 - y1)*(x2 - x1))*
        ((x4 - x1)*(y2 - y1) - (y4 - y1)*(x2 - x1))) >0){
            return 1;
        }
    else {
        return 0;
    } 
}

int rgen_positive(int a){
    int ind=0;
	FILE *fp;
	fp = fopen("/dev/urandom", "r");
    while(ind<a){
    fread(&ind, 1, sizeof(ind),fp);
    }
    return ind;
}

int rgen_vertex(int a){
    int ind,ind_true;
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    fread(&ind, 1, sizeof(ind),fp);
    ind_true=ind%(a+1); 
    return ind_true;
}

string unique_name(int length_name) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string str;
    int name_a;
    for (int i=0; i<length_name; i++){
       name_a=rand()%26;
       str.push_back(charset[name_a]);
    }
    return str;
}

void ending(int a){
    if (a>25) {
        fprintf( stderr, "Error: can't find the vertices within 25 times\n" );
        exit(0);
    }
}

int main(int argc, char* argv[]){
    int s,n,l,c;
    string s_gen,n_gen,l_gen,c_gen;
    int command_rgen;
    const char *optstring = "s:n:l:c:";
    while ((command_rgen = getopt(argc, argv, optstring))!= -1){
        switch(command_rgen){
        case 's':
            s_gen = optarg;
            break;
        case 'n':
            n_gen = optarg;
            break;
        case 'l':
            l_gen = optarg;
            break;
        case 'c':
            c_gen = optarg;
            break;
        case '?':
            cerr << "Error: option -" << optopt << " requires an argument."<< endl;

            return 1;
        }
    }
    s=stoi(s_gen);
    n=stoi(n_gen);
    l=stoi(l_gen);
    c=stoi(c_gen);
    
    while(true){
        int sgen,ngen,lgen,cgen;
        int stop=0;
        graph main0;
        sgen=rgen_positive(2)%(s-1)+2;//the number of streets
        for (int i=0; i<sgen; i++){
            street s = street();
            s.street_name = unique_name(3+i);
            main0.streets.push_back(s);
            ngen=rgen_positive(1)%n+1;
            for (int j=0; j<ngen; ){
                int torf=0, lapping_other=0;
                graph segment;
                if (j==0){
                    int lapping1=0;
                    segment.x1=rgen_vertex(c);
                    segment.y1=rgen_vertex(c);
                    segment.x2=rgen_vertex(c);
                    segment.y2=rgen_vertex(c);
                    if (segment.same_points1()==0){
                        stop+=1;
                        ending(stop);
                        continue;
                    }
                    else{
                        for (int m=0; m<i ; m++){
                            for (int n=0; n<(main0.streets[m].street_member).size()/2-1;n++){
                                segment.x3=main0.streets[m].street_member[2*n];
                                segment.y3=main0.streets[m].street_member[2*n+1];
                                segment.x4=main0.streets[m].street_member[2*n+2];
                                segment.y4=main0.streets[m].street_member[2*n+3];
                                if (segment.if_vertical1()==0 && segment.if_vertical2()==0){
                                    if (segment.vertical_overlapping()==0){
                                        lapping1=1;
                                        stop+=1;
                                        ending(stop);
                                        break;
                                    }
                                    else {
                                        continue;
                                    }
                                }
                                else if (segment.if_vertical1()==0 || segment.if_vertical2()==0){
                                    continue;
                                }
                                else {
                                    segment.xy_formula1();
                                    segment.xy_formula2();
                                    if (segment.if_overlapping()==0){
                                        lapping1=1;
                                        stop+=1;
                                        ending(stop);
                                        break;
                                    }
                                    else {
                                        continue;
                                    }
                                }
                            }
                            if (lapping1==1){
                                break;
                            }
                            else {
                                continue;
                            }
                        }
                        if (lapping1==1){
                            continue;
                        }
                        else {
                            main0.streets[i].street_member.push_back(segment.x1);
                            main0.streets[i].street_member.push_back(segment.y1);
                            main0.streets[i].street_member.push_back(segment.x2);
                            main0.streets[i].street_member.push_back(segment.y2);
                            s.street_member.push_back(segment.x1);
                            s.street_member.push_back(segment.y1);
                            s.street_member.push_back(segment.x2);
                            s.street_member.push_back(segment.y2);
                            j+=1; 
                            continue;
                        }
                    } 
                }
                else{
                    segment.x4=rgen_vertex(c);
                    segment.y4=rgen_vertex(c);
                    segment.x3=s.street_member[(s.street_member).size()-2];
                    segment.y3=s.street_member[(s.street_member).size()-1];
                    if (segment.same_points2()==0){
                        stop+=1;
                        ending(stop);
                        continue;
                    }
                    for (int a=0; a<(s.street_member).size()/2-1 ; a++){
                        segment.x1=s.street_member[2*a];
                        segment.y1=s.street_member[2*a+1];
                        segment.x2=s.street_member[2*a+2];
                        segment.y2=s.street_member[2*a+3];
                        if (segment.if_vertical1()==0 && segment.if_vertical2()==0){
                            if (segment.vertical_overlapping()==0){
                                torf=1;
                                stop+=1;
                                ending(stop);
                                break;
                            }
                            else {
                                continue;
                            }
                        }
                        else if (segment.if_vertical1()==0 || segment.if_vertical2()==0){
                            if (segment.x2==segment.x3 && segment.y2==segment.y3){
                                continue;
                            }
                            else{
                                if (segment.inter_sect()==0){
                                    torf=1;
                                    stop+=1;
                                    ending(stop);
                                    break;
                                }
                                else {
                                    continue;
                                }
                            }
                        }
                        else {
                            segment.xy_formula1();
                            segment.xy_formula2();
                            if (segment.if_overlapping()==0){
                                torf=1;
                                stop+=1;
                                ending(stop);
                                break;
                            }
                            else if (segment.x2==segment.x3 && segment.y2==segment.y3){
                                continue;
                            }
                            else {
                                if (segment.inter_sect()==0){
                                    torf=1;
                                    stop+=1;
                                    ending(stop);
                                    break;
                                }
                                else {
                                    continue;
                                }
                            }
                        }
                    }
                    for (int b=0; b<i; b++ ){
                        for (int c=0; c<(main0.streets[b].street_member).size()/2-1;c++){
                            segment.x1=main0.streets[b].street_member[2*c];
                            segment.y1=main0.streets[b].street_member[2*c+1];
                            segment.x2=main0.streets[b].street_member[2*c+2];
                            segment.y2=main0.streets[b].street_member[2*c+3];
                            if (segment.if_vertical1()==0 && segment.if_vertical2()==0){
                                if (segment.vertical_overlapping()==0){
                                    lapping_other=1;
                                    stop+=1;
                                    ending(stop);
                                    break;
                                }
                                else {
                                    continue;
                                }
                            }
                            else if (segment.if_vertical1()==0 || segment.if_vertical2()==0){
                                continue;
                            }
                            else {
                                segment.xy_formula1();
                                segment.xy_formula2();
                                if (segment.if_overlapping()==0){
                                    lapping_other=1;
                                    stop+=1;
                                    ending(stop);
                                    break;
                                }
                                else {
                                    continue;
                                }
                            }
                        }
                        if (lapping_other==1){
                            break;
                        }
                        else {
                            continue;
                        }
                    }
                    if (torf==0 && lapping_other==0){
                        main0.streets[i].street_member.push_back(segment.x4);
                        main0.streets[i].street_member.push_back(segment.y4);
                        s.street_member.push_back(segment.x4);
                        s.street_member.push_back(segment.y4);
                        j+=1;
                        continue;
                    }
                    else {
                        continue;
                    }
                }
            }
        }
        
        for (int i=0;i<(main0.streets).size();i++){
            cout<<"add "<<"\""<<main0.streets[i].street_name<<"\"";
            for (int j=0; j<(main0.streets[i].street_member).size(); j++){
                if (j%2==0){
                    cout<<" ("<<main0.streets[i].street_member[j];
                }
                else {
                    cout<<","<<main0.streets[i].street_member[j]<<")";
                }
            }
            cout<<endl;
        }

        cout<<"gg"<<endl;
        sleep(rgen_positive(5)%(l-4)+5);
        for (int x=0; x<(main0.streets).size(); x++){
            cout<<"rm "<<"\""<<main0.streets[x].street_name<<"\""<<endl;
        }
    }
    return 0;
}
