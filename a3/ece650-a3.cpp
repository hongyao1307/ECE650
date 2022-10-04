#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;


int main (int argc, char* argv[]) {
    int command;
    string sgen,ngen,lgen,cgen;
    vector <pid_t> children;
    const char *optstring = "s:n:l:c:";
    while ((command = getopt(argc, argv, optstring))!= -1){
        switch (command){
        case 's':
            sgen = optarg;
            break;
        case 'n':
            ngen = optarg;
            break;
        case 'l':
            lgen = optarg;
            break;
        case 'c':
            cgen = optarg;
            break;
        case '?':
            cerr << "Error: option -" << optopt << " requires an argument."<< endl;

            return 1;
        }
    }

    int s=0;
    if (sgen.size()!=0){
        s = stoi(sgen);
        if (s<2){
            fprintf(stderr, "Error: the input s is too small\n" );
            exit(0);
        }
    }
    else {
        sgen="10";
    }
    int n=0;
    if (ngen.size()!=0){
        n = stoi(ngen);
        if (n<1){
            fprintf(stderr, "Error: the input n is too small\n" );
            exit(0);
        }
    }
    else {
        ngen="5";
    }
    int l=0;
    if (lgen.size()!=0){
        l = stoi(lgen);
        if (l<5){
            fprintf(stderr, "Error: the input l is too small\n" );
            exit(0);
        }
    }
    else {
        lgen="5";
    }
    int c=0;
    if (cgen.size()!=0){
        c = stoi(cgen);
        if (c<1){
            fprintf(stderr, "Error: the input c is too small\n" );
            exit(0);
        }
    }
    else {
        cgen="20";
    }
    
    int toA1[2], toA2[2];

    if (pipe(toA1)==-1) {
        fprintf(stderr, "Error: Pipe Failed\n" );
        return 1;
    }
    if (pipe(toA2)==-1){
        fprintf(stderr, "Error: Pipe Failed\n" );
        return 1;
    }

    pid_t pid;
    pid = fork();

    if (pid <0){
        fprintf(stderr, "Error: Fork Failed\n" );
        return 1;
    }
    else if (pid ==0){
        dup2(toA1[1], STDOUT_FILENO);//redirect stdout to the pipe
        close(toA1[0]);
        close(toA1[1]);
        char* args[6];
        args[0] = (char *)"rgen";
        args[1] = (char *)("-s "+sgen).c_str();
        args[2] = (char *)("-n "+ngen).c_str();
        args[3] = (char *)("-l "+lgen).c_str();
        args[4] = (char *)("-c "+ngen).c_str();
        args[5] = nullptr;
        int running;
        running= execv("./rgen", args);
    }
    children.push_back(pid);

    pid = fork();
    if (pid<0){
        fprintf(stderr, "Error: Fork Failed\n" );
        return 1;
    }
    else if (pid==0){
        dup2(toA1[0], STDIN_FILENO);//redirect stdin from the pipe
        close(toA1[1]);
        close(toA1[0]);
        dup2(toA2[1], STDOUT_FILENO);// redirect stdout to the pipe
        close(toA2[0]);
        close(toA2[1]);
        char* args[3];
        args[0] = (char*)"/usr/bin/python3";
        args[1] = (char*)"./ece650-a1.py";
        args[2] = nullptr;
        int running;
        running = execv("/usr/bin/python3", args);
    }
    children.push_back(pid);

    pid = fork();
    if (pid<0){
        fprintf(stderr, "Error: Fork Failed\n" );
        return 1;
    }
    else if (pid==0){
        dup2(toA2[0], STDIN_FILENO);// redirect stdin from the pipe
        close(toA2[1]);
        close(toA2[0]);
        char *args[2];
        args[0] = (char *)"ece650-a2";
        args[1] = nullptr;
        int running;
        running = execv("./ece650-a2", args);
    }
    children.push_back(pid);

    dup2(toA2[1], STDOUT_FILENO);//a pipe to a2
    close(toA2[0]);
    close(toA2[1]);
    string  sline;
    while (getline(cin, sline)){//read the input and pass it to a2
        cout<<sline<<endl;
    }

    for (int i=0; i<children.size(); i++) {
        pid_t k = children[i];
        int status;
        kill(k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}

