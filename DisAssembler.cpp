#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>  
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
using namespace std;

unordered_map<string, string> createDestTable(){
    unordered_map<string, string> destTable;
    destTable["001"] = "M";
    destTable["000"] = "";
    destTable["010"] = "D";
    destTable["011"] = "MD";
    destTable["100"] = "A";
    destTable["101"] = "AM";
    destTable["110"] = "AD"; 
    destTable["111"] = "AMD";
    return destTable;
}

unordered_map<string, string> createJumpTable(){
    unordered_map<string, string> jumpTable;
    jumpTable["001"] = "JGT";
    jumpTable["000"] = "";
    jumpTable["010"] = "JEQ";
    jumpTable["011"] = "JGE";
    jumpTable["100"] = "JLT";
    jumpTable["101"] = "JNE";
    jumpTable["110"] = "JLE";
    jumpTable["111"] = "JMP";
    return jumpTable;
}

unordered_map<string, string> createCompTable(){
    unordered_map<string, string> compTable;
    compTable["0101010"] = "0";
    compTable["0111111"] = "1";
    compTable["0111010"] = "-1";
    compTable["0001100"] = "D";
    compTable["0110000"] = "A";
    compTable["1110000"] = "M";
    compTable["0001101"] = "!D";
    compTable["0110001"] = "!A";
    compTable["1110001"] = "!M";
    compTable["0001111"] = "-D";
    compTable["0110011"] = "-A";
    compTable["1110011"] = "-M";
    compTable["0011111"] = "D+1";
    compTable["0110111"] = "A+1";
    compTable["1110111"] = "M+1";
    compTable["0001110"] = "D-1";
    compTable["0110010"] = "A-1";
    compTable["1110010"] = "M-1";
    compTable["0000010"] = "D+A";
    compTable["1000010"] = "D+M";
    compTable["0010011"] = "D-A";
    compTable["1010011"] = "D-M";
    compTable["0000111"] = "A-D";
    compTable["1000111"] = "M-D";
    compTable["0000000"] = "D&A";
    compTable["1000000"] = "D&M";
    compTable["0010101"] = "D|A";
    compTable["1010101"] = "D|M";
    return compTable;
}

string binaryToInt(string& x){
    int val = 0;
    stringstream ss;
    
    ss << x[14];
    int x14;
    ss >> x14;
    val += x14 * 1;
   
    
    ss.str("");
    ss.clear();
    ss << x[13];
    int x13; ss >> x13;
    val += x13 * 2;
    
    
    ss.str("");
    ss.clear();
    ss << x[12];
    int x12; ss >> x12;
    val += x12 * pow(2,2);

    ss.str("");
    ss.clear();
    ss << x[11];
    int x11; ss >> x11;
    val += x11 * pow(2,3);

    ss.str("");
    ss.clear();
    ss << x[10];
    int x10; ss >> x10;
    val += x10 * pow(2,4);

    ss.str("");
    ss.clear();
    ss << x[9];
    int x9; ss >> x9;
    val += x9 * pow(2,5);
    
    ss.str("");
    ss.clear();
    ss << x[8];
    int x8; ss >> x8;
    val += x8 * pow(2,6);
    
    ss.str("");
    ss.clear();
    ss << x[7];
    int x7; ss >> x7;
    val += x7 * pow(2,7);
    
    ss.str("");
    ss.clear();
    ss << x[6];
    int x6; ss >> x6;
    val += x6 * pow(2,8);
    
    ss.str("");
    ss.clear();
    ss << x[5];
    int x5; ss >> x5;
    val += x5 * pow(2,9);

    ss.str("");
    ss.clear();
    ss << x[4];
    int x4; ss >> x4;
    val += x4 * pow(2,10);
    
    ss.str("");
    ss.clear();
    ss << x[3];
    int x3; ss >> x3;
    val += x3 * pow(2,11);
    
    ss.str("");
    ss.clear();
    ss << x[2];
    int x2; ss >> x2;
    val += x2 * pow(2,12);

    ss.str("");
    ss.clear();
    ss << x[1];
    int x1; ss >> x1;
    val += x1 * pow(2,13);

    ss.str("");
    ss.clear();
    ss << x[0];
    int x0; ss >> x0;
    val += x0 * pow(2,14);
    
    string final = to_string(val);
    return final;
}


int main() {
    string line;
    string label;
    string bintoInstr;
    unordered_map<string,string> d = createDestTable();
    unordered_map<string,string> j = createJumpTable();
    unordered_map<string,string> c = createCompTable();
    
    string comp, dest, jump, comp1, dest1, jump1, ain, temp;
    
    while(getline(cin, line)){
        label = line;
        if(label[0] == '0'){
            temp = label;
            temp = temp.erase(0,1);
            ain = binaryToInt(temp);
            cout << "@" << ain << endl;
        }
        
        if(label[0] == '1'){
            comp1=""; dest1=""; jump1="";
            for(unsigned int i = 3; i<label.size();i++){
                if(i < 10)
                    comp1 += label[i];
                
                else if(i>9 && i < 13)
                    dest1 += label[i];
                
                else if(i>12)
                    jump1 += label[i];
            }
            comp = c[comp1];
            dest = d[dest1];
            jump = j[jump1];
            if(dest == "" && jump == ""){
                bintoInstr = comp;
            }
            else if(dest == ""){
                bintoInstr = comp + ";" + jump;
            }
            else if(jump == ""){
                bintoInstr = dest + "=" + comp;
            }
            else{
                bintoInstr = dest + "=" + comp + ";" + jump;
            }
            cout << bintoInstr << endl;
        }
        
    }
    return 0;
}