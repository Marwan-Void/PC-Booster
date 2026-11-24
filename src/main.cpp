#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <cstdlib>
using namespace std;
namespace fs = filesystem;

fs::path getPath(){
    const char* env = getenv("TEMP");
    if(!env || *env == '\0') env = getenv("TMP");
    if(!env || *env == '\0') return {};

    fs::path path(env);
    error_code ec;
    if(!fs::exists(path, ec) || !fs::is_directory(path, ec)) return {};
    return path;
}

void boostFunc(){
    fs::path path = getPath();
    if(path.empty()){
        cerr << "error: could not locate %TEMP% directory.\n";
        return;
    }
    size_t allCount = 0;
    size_t deletedCount = 0;
    size_t skippedCount = 0;
    uintmax_t allBytes = 0;
    uintmax_t freedBytes = 0;

    error_code ec;
    for(const auto& entry : fs::directory_iterator(path, ec)){
        ++allCount;
        if(ec){
            ++skippedCount;
            ec.clear();
            continue;
        }

        uintmax_t entrySize = 0;
        bool isDir = entry.is_directory(ec);
        if(ec){
            ++skippedCount;
            ec.clear();
            continue;
        }

        if(!isDir){
            entrySize = entry.file_size(ec);
            if(ec){
                entrySize = 0;
                ec.clear();
            }
        } else {
            for(const auto& subEntry : fs::recursive_directory_iterator(entry.path(), ec)){
                if(ec){
                    ec.clear();
                    continue;
                }

                if(subEntry.is_regular_file(ec) && !ec){
                    uintmax_t sz = subEntry.file_size(ec);
                    if(!ec){
                        entrySize += sz;
                    }
                }
                ec.clear();
            }
        }

        allBytes += entrySize;
        fs::remove_all(entry.path(), ec);
        if(!ec){
            ++deletedCount;
            freedBytes += entrySize;
        } else {
            ++skippedCount;
            ec.clear();
        }
    }

    cout << "-----------------------------------------------\n";
    cout << "all items: " << allCount << "\n";
    cout << "all size before deletion: " << static_cast<double>(allBytes) / (1024.0 * 1024.0) << "\n";
    cout << "deleted items: " << deletedCount << "\n";
    cout << "skipped items: " << skippedCount << "\n";
    cout << "freed space:   " << static_cast<double>(freedBytes) / (1024.0 * 1024.0) << " MB\n";
    cout << "boost completed!\n";
    cout << "-----------------------------------------------\n";
}

int main(){
    string inp;
    while(true){
        cout << ">>> ";
        if(!getline(cin, inp)) break;
        stringstream ss(inp);
        string cmd, arg;
        ss >> cmd >> arg;

        if(cmd.empty()) continue;
        else if(cmd == "exit") break;
        else if(cmd == "help"){
            if(arg.empty()){
                cout << "-----------------------------------------------\n";
                cout << "commands:\n";
                cout << "1. help: show all commands\n";
                cout << "2. help [command]: show info about this command\n";
                cout << "3. boost: boost your device (main function)\n";
                cout << "4. exit: quit the program\n";
                cout << "-----------------------------------------------\n";
            } 
            else if(arg == "boost") cout << "boost: boost your device (main function)\n";
            else if(arg == "exit") cout << "exit: quit the program\n";
            else if(arg == "help") cout << "help: show all commands\n";
            else cerr << "error: unknown command,\nuse \"help\" to check commands\n";
        } 
        else if(cmd == "boost") boostFunc();
        else cerr << "error: unknown command,\nuse \"help\" command to check commands\n";
        // * we wanna handle help [command] 
    }
    return 0;
}