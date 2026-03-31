#include <bits/stdc++.h>
#include "src.hpp"
using namespace std;

string get_data(istream &in) {
    string ret="", line;
    while (getline(in, line)) {
        if (line.size()==0) continue;
        if (line=="#####") break;
        ret += line + "\n";
    }
    return ret;
}

int main(){
    // Sample from README
    string input =
        "ICPCJudger\n"
        "1000 1000\n"
        "abcd\n"
        "dcba\n"
        "#####\n"
        "500 1001\n"
        "abcd\n"
        "dcba\n"
        "#####\n"
        "500 500\n"
        "abcd\n"
        "dcba\n"
        "#####\n";
    istringstream in(input);
    string judger_name; in>>judger_name; size_t tl, ml; in>>tl>>ml; string tmp;
    BaseJudger *judger=nullptr;
    if (judger_name=="ICPCJudger"){
        tmp = get_data(in); judger = new ICPCJudger(tl, ml, tmp.c_str());
    }
    size_t st, sm; while (in>>st>>sm){ tmp = get_data(in); judger->Submit(st, sm, tmp.c_str()); }
    cout<<judger->GetScore()<<"\n"; delete judger; return 0;
}
