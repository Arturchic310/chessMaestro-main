#include<iostream>
#include<string>
#include<unordered_map>
using namespace std;
int main(){
    unordered_map<string, int> table;
    for(;;){
        string str;
        cin >> str;
        if(str == "quit")
        return 0;
        if(str == "check")
        break;
        if(str=="clear")
            table.clear();
        int nodes;
        cin >> nodes;
        
        if(table.contains(str)){
            if(table[str] != nodes){
                cout << "ERROR: " << str << "== " << table[str] << " not " << nodes << "\n";
                
            }
        }
        table[str] = nodes;
    }
    for(;;){
        string str;
        cin >> str;
        if(str == "check")
            break;
        int nodes;
        cin >> nodes;
        
        if(table.contains(str)){
            if(table[str] != nodes){
                cout << "ERROR: " << str << "== " << table[str] << " not " << nodes << "\n";
                
            }
            if(table[str] == -1){
                cout << "ERROR: " << str << " too many\n";
            }
            table[str] = -1;
        }
        if(!table.contains(str)){
            cout << "ERROR: " << str << " not match\n";
        }
    }
    for(auto &i : table){
        if(i.second != -1){
            cout << "ERROR: " << i.first << " not match\n";
        }
    }
}