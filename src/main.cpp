#include <iostream>
#include <string>
using namespace std;

int main(){
    while(true){
        string inp;
        cout << ">>> ";
        cin >> inp;

        if(inp == "exit") break;
        else if(inp.empty()) continue;
        else if(inp == "help"){
            cout << "-----------------------------------------------\n";
            cout << "commands:\n";
            cout << "1. help: show all commands\n";
            cout << "2. help [command]: show info about this command\n";
            cout << "3. boost: boost your device (main function)\n";
            cout << "4. exit: quit the program\n";
            cout << "-----------------------------------------------\n";
        } else if(inp == "boost"){
            cout << "coming soon.\n";
        } else {
            cerr << "error: unknown command,\nuse \"help\" command to check commands\n";
        }
    }
    return 0;
}