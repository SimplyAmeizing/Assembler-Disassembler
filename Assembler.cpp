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

unordered_map<string, int> createSymbolTable(){
    unordered_map<string, int> symbolTable;
    symbolTable["R0"] = 0;
    symbolTable["R1"] = 1;
    symbolTable["R2"] = 2;
    symbolTable["R3"] = 3;    
    symbolTable["R4"] = 4;
    symbolTable["R5"] = 5; 
    symbolTable["R6"] = 6;
    symbolTable["R7"] = 7;
    symbolTable["R8"] = 8;
    symbolTable["R9"] = 9;    
    symbolTable["R10"] = 10;
    symbolTable["R11"] = 11;
    symbolTable["R12"] = 12;
    symbolTable["R13"] = 13;
    symbolTable["R14"] = 14;    
    symbolTable["R15"] = 15;
    symbolTable["SP"] = 0;
    symbolTable["LCL"] = 1;
    symbolTable["ARG"] = 2;
    symbolTable["THIS"] = 3;    
    symbolTable["THAT"] = 4;
    symbolTable["SCREEN"] = 16384;
    symbolTable["KBD"] = 24576;
    
    return symbolTable;
}

unordered_map<string, string> createDestTable(){
    unordered_map<string, string> destTable;
    destTable["M"] = "001";
    destTable["null"] = "000";
    destTable["D"] = "010";
    destTable["MD"] = "011";
    destTable["DM"] = "011";
    destTable["A"] = "100";
    destTable["AM"] = "101";
    destTable["MA"] = "101";
    destTable["AD"] = "110"; 
    destTable["DA"] = "110"; 
    destTable["AMD"] = "111";
    destTable["ADM"] = "111";
    destTable["DAM"] = "111";
    destTable["DMA"] = "111";
    destTable["MDA"] = "111";
    destTable["MAD"] = "111";
    return destTable;
}

unordered_map<string, string> createJumpTable(){
    unordered_map<string, string> jumpTable;
    jumpTable["JGT"] = "001";
    jumpTable["null"] = "000";
    jumpTable["JEQ"] = "010";
    jumpTable["JGE"] = "011";
    jumpTable["JLT"] = "100";
    jumpTable["JNE"] = "101";
    jumpTable["JLE"] = "110";
    jumpTable["JMP"] = "111";
    return jumpTable;
}

unordered_map<string, string> createCompTable(){
    unordered_map<string, string> compTable;
    compTable["0"] = "0101010";
    compTable["1"] = "0111111";
    compTable["-1"] = "0111010";
    compTable["D"] = "0001100";
    compTable["A"] = "0110000";
    compTable["M"] = "1110000";
    compTable["!D"] = "0001101";
    compTable["!A"] = "0110001";
    compTable["!M"] = "1110001";
    compTable["-D"] = "0001111";
    compTable["-A"] = "0110011";
    compTable["-M"] = "1110011";
    compTable["D+1"] = "0011111";
    compTable["A+1"] = "0110111";
    compTable["M+1"] = "1110111";
    compTable["D-1"] = "0001110";
    compTable["A-1"] = "0110010";
    compTable["M-1"] = "1110010";
    compTable["D+A"] = "0000010";
    compTable["A+D"] = "0000010";
    compTable["D+M"] = "1000010";
    compTable["M+D"] = "1000010";
    compTable["D-A"] = "0010011";
    compTable["D-M"] = "1010011";
    compTable["A-D"] = "0000111";
    compTable["M-D"] = "1000111";
    compTable["D&A"] = "0000000";
    compTable["A&D"] = "0000000";
    compTable["D&M"] = "1000000";
    compTable["M&D"] = "1000000";
    compTable["D|A"] = "0010101";
    compTable["A|D"] = "0010101";
    compTable["D|M"] = "1010101";
    compTable["M|D"] = "1010101";
    return compTable;
}



string removeWhiteSpaceComments(string inp){
    string wc = "";
    for(unsigned int i=0; i<inp.size();i++){
        if(inp[i] == ' ')
            continue;
        else if(inp[i] == '/' && inp[i+1] == '/')
            break;
        
        wc += inp[i]; 
    }
    return wc;
}

bool isValidSymbol(string drop){
    for(unsigned int i=0; i<drop.size();i++){
        if(drop[i] == '$' || drop[i] == ':' || drop[i] == '_' || 
        drop[i] == '.' || isalpha(drop[i]) || isdigit(drop[i]) )
            return true;
    }
    return false;
}

bool isPositiveInteger(string dropAt){
    for(unsigned int j=0; j<dropAt.size(); j++){
        if(!isdigit(dropAt[j]))
            break;
        if(isdigit(dropAt[j]) && j == (dropAt.size() - 1))
            return true;
    }
    return false;
}

bool isNotNumber(string a){
    if(isdigit(a[1]))
        return false;
    return true;    
}

string toBinary15(string dropAt){
    string drop="";
    string d="";
    
    for(unsigned int i=0;i<dropAt.size();i++)
        d += dropAt[i];
        
    int d1 = stoi(d);
    
    while(d1 > 0){
        if(d1 % 2 == 1)
            drop+= '1';
        else
            drop += '0';
        d1 /= 2;
    }
    int rem = 15 - drop.size();
    if(rem > 0){
        for(int j=0; j<rem;j++){
            drop += '0';
        }
    }
    reverse(drop.begin(),drop.end());
    return drop;
}

bool isAinstruction(string ainstr){
    int asize = ainstr.size();
    string dropAt="";
    if(ainstr[0] != '@')
        return false;
        
    for(int i=0; i<asize;i++){
        if(i == 0)
            continue;
        dropAt += ainstr[i];
    }
    
    if(isPositiveInteger(dropAt))
        return true;
    if(isdigit(dropAt[0]))
        return false;
    if(isValidSymbol(dropAt))
        return true;
    
    return false;
}

string toAinstruction(string con, unordered_map<string,int> x){
    int asize = con.size();
    string dropAt="";
    string bin="";
    for(int i=0; i<asize;i++){
        if(i == 0)
            continue;
        dropAt += con[i];
    }
    if(isPositiveInteger(dropAt)){
        bin = toBinary15(dropAt);
        return '0' + bin;
    }
    
    int addr = x[dropAt];
    string addr1 = to_string(addr);
    bin = toBinary15(addr1);
    return '0' + bin;
}

bool isCinstruction(string cinstr){
    unordered_map<string,string> d = createDestTable();
    unordered_map<string,string> j = createJumpTable();
    unordered_map<string,string> c = createCompTable();
    int counts = 0;
    int counteq = 0;
    for(unsigned int i=0; i<cinstr.size(); i++){
        if(cinstr[i] == ';')
            counts++;
        if(cinstr[i] == '=')
            counteq++;
    }
    
    if((counts > 1) || (counteq > 1))
        return false;
        
    bool checker = false;
    bool checker1 = false;
    string dest="";
    string comp="";
    string jump="";
    
    if(counts == 0 && counteq == 0){
        for(unsigned int j=0; j<cinstr.size();j++){
                comp += cinstr[j];
            }
            if(c.find(comp) == c.end())
                return false;
        }        
    
    if(counts == 0 && counteq == 1){
        for(unsigned int j=0; j<cinstr.size();j++){
            if(cinstr[j] == '='){
                checker = true;
                continue;
            }
            if(checker){
                comp += cinstr[j];
                continue;
            }
            dest += cinstr[j];
        }
        if(d.find(dest) == d.end())
            return false;
        if(c.find(comp) == c.end())
            return false;
    }
    if(counts == 1 && counteq == 0){
        for(unsigned int j=0; j<cinstr.size();j++){
            if(cinstr[j] == ';'){
                checker = true;
                continue;
            }
            if(checker){
                jump += cinstr[j];
                continue;
            }
            comp += cinstr[j];        
        }
        if(c.find(comp) == c.end())
            return false;
        if(j.find(jump) == j.end())
            return false;
    }
    if(counts == 1 && counteq == 1){
        for(unsigned int j=0; j<cinstr.size();j++){
            if(cinstr[j] == '='){
                checker = true;
                continue;
            }
            else if(cinstr[j] == ';'){
                checker1 = true;
                continue;
            }
            if(checker && !checker1){
                comp+=cinstr[j];
                continue;
            }
            if(checker && checker1){
                jump += cinstr[j];
                continue;
            }
        dest += cinstr[j];
        }
        if(d.find(dest) == d.end())
            return false;
        if(c.find(comp) == c.end())
            return false;
        if(j.find(jump) == j.end())
            return false;    
    }
    return true;
}

string toCinstruction(string c){
    unordered_map<string,string> d = createDestTable();
    unordered_map<string,string> j = createJumpTable();
    unordered_map<string,string> c1 = createCompTable();
    string prefix = "111";
    int counts = 0;
    int counteq = 0;
    for(unsigned int i=0; i<c.size(); i++){
        if(c[i] == ';')
            counts++;
        if(c[i] == '=')
            counteq++;
    }    
    bool checker = false;
    bool checker1 = false;
    string dest1="";
    string comp1="";
    string jump1="";
    string dest="";
    string comp="";
    string jump="";
    
    if(counts == 0 && counteq == 0){
        for(unsigned int j=0; j<c.size();j++){
                comp1 += c[j];
            }
            jump += "000";
            comp += c1[comp1];
            dest += "000";
            if(comp == "null")
                throw invalid_argument("incorrect syntax");
    }  
    
    
    if(counts == 0 && counteq == 1){
        for(unsigned int j=0; j<c.size();j++){
            if(c[j] == '='){
                checker = true;
                continue;
            }
            if(checker){
                comp1 += c[j];
                continue;
            }
            dest1 += c[j];
        }
        jump += "000";
        comp += c1[comp1];
        dest += d[dest1];
        if(comp == "null" || dest == "null"){
            throw invalid_argument( "incorrect syntax" );
        }
    }    
    if(counts == 1 && counteq == 0){
        for(unsigned int j=0; j<c.size();j++){
            if(c[j] == ';'){
                checker = true;
                continue;
            }
            if(checker){
                jump1 += c[j];
                continue;
            }
            comp1 += c[j];        
        }
        dest += "000";
        comp += c1[comp1];
        jump += j[jump1];
        if(comp == "null" || jump == "null"){
            throw invalid_argument( "incorrect syntax" );
        }
    }
    if(counts == 1 && counteq == 1){
        for(unsigned int j=0; j<c.size();j++){
            if(c[j] == '='){
                checker = true;
                continue;
            }
            else if(c[j] == ';'){
                checker1 = true;
                continue;
            }
            if(checker && !checker1){
                comp1 +=c[j];
                continue;
            }
            if(checker && checker1){
                jump1 += c[j];
                continue;
            }
        dest1 += c[j];
        }
        dest += d[dest1];
        comp += c1[comp1];
        jump += j[jump1];
        if(comp == "null" || jump == "null" || dest == "null")
            throw invalid_argument( "incorrect syntax" );  
    }    
    return prefix + comp + dest + jump;

}

bool isLabel(string lab){
    if(lab.size() > 2){
        if(lab[0] == '(' && lab[lab.size()-1] == ')')
            return true;
    }
    return false;
}


string removeParen(string paren){
    string paren1 = "";
    for(unsigned int i=0; i<paren.size();i++){
        if(i == 0 || i == paren.size()-1)
            continue;    
        paren1 += paren[i];
    }
    return paren1;
}

bool isEmptyLine(string line){
    if(line == "")
        return true;
    return false;
}

int main() {
    string line;
    unordered_map<string, int> symbolTable = createSymbolTable();
    
    int pc = 0;
    string label;
    stringstream ss;
    string clean;
    string bin;
    //first runthrough
    while(getline(cin, line)){
        clean = removeWhiteSpaceComments(line);
        
        if(isLabel(clean)){
            label = removeParen(clean);
            if(symbolTable.find(label) == symbolTable.end())
                symbolTable[label] = pc;
        }
        
        if(isAinstruction(clean) || isCinstruction(clean))
            pc += 1;
        ss << clean << endl;
    }
    
    string runtwo, temp, temp1;
    int nextAdd = 16;
    while(getline(ss,runtwo)){
        clean = removeWhiteSpaceComments(runtwo);
        
        if(isEmptyLine(clean))
            continue;
       
        if(isAinstruction(clean)){
            temp1 = clean;
            temp = temp1.erase(0,1);
            
            if(!isPositiveInteger(temp) && (symbolTable.find(temp) == symbolTable.end())){
                symbolTable[temp] = nextAdd;
                nextAdd+=1;
            }
            bin = toAinstruction(clean, symbolTable);
            cout << bin << endl;
            continue;
        }
        
        if(isCinstruction(clean)){
            bin = toCinstruction(clean);
            cout << bin << endl;
            continue;
        }
        
        
    }
    return 0;
}
