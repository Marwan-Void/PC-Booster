#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>
#include <cctype>
using namespace std;
namespace fs = filesystem;

struct CmdInfo {
    const char* name;
    const char* desc;
};

static const CmdInfo commands[] = {
    {"help", "help: show all commands"},
    {"boost temp", "boost temp: clean %temp% folder"},
    {"boost bin", "boost bin: clear recycle bin"},
    {"boost cache", "boost cache: clear browser cache"},
    {"boost", "boost: do all boosts"},
    {"status", "status: show device status"},
    {"exit", "exit: quit the program"}
};

string getProgId(){
    HKEY mainKey = nullptr;
    const wchar_t* subKey = L"Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\http\\UserChoice";
    if(RegOpenKeyExW(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &mainKey) != ERROR_SUCCESS) return "";

    wchar_t progId[256] = {0};
    DWORD bufferSize = sizeof(progId);
    DWORD type = 0;
    LONG res = RegQueryValueExW(mainKey, L"ProgId", nullptr, &type, reinterpret_cast<LPBYTE>(progId), &bufferSize);

    RegCloseKey(mainKey);
    if(res != ERROR_SUCCESS || type != REG_SZ) return "";

    wstring wProg(progId);
    string result(wProg.begin(), wProg.end());
    for(char& ch : result) ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    return result;
}

fs::path getPath(const string& target){
    error_code ec;
    if(target == "TEMP"){
        const char* env = getenv("TEMP");
        if(!env || *env == '\0') env = getenv("TMP");
        if(!env || *env == '\0') return {};

        fs::path path(env);
        if(!fs::exists(path, ec) || !fs::is_directory(path, ec)) return {};
        return path;
    } else if(target == "cache"){
        const char* testEnv = getenv("TEST_CACHE_PATH");
        if(testEnv && *testEnv != '\0'){
            fs::path testPath(testEnv);
            if(fs::exists(testPath, ec) && fs::is_directory(testPath, ec)) return testPath;
            return {};
        }

        const char* localAppData = getenv("LOCALAPPDATA");
        if(!localAppData || *localAppData == '\0') return {};
        fs::path base(localAppData);

        string progId = getProgId();
        fs::path cachePath;

        if(progId.find("edge") != string::npos) cachePath = base / "Microsoft" / "Edge" / "User Data" / "Default" / "Cache" / "Cache_Data";
        else if(progId.find("chrome") != string::npos) cachePath = base / "Google" / "Chrome" / "User Data" / "Default" / "Cache" / "Cache_Data";
        else return {};

        if(fs::exists(cachePath, ec) && fs::is_directory(cachePath, ec)) return cachePath;
        return {};
    } return {};
}

void cleanBin() {
    SHQUERYRBINFO rbInfo{};
    rbInfo.cbSize = sizeof(SHQUERYRBINFO);
    HRESULT qHr = SHQueryRecycleBinA(nullptr, &rbInfo);
    if(SUCCEEDED(qHr) && rbInfo.i64NumItems == 0){
        cout << "Recycle Bin is already empty.\n";
        return;
    }

    double freedMB = SUCCEEDED(qHr)
        ? static_cast<double>(rbInfo.i64Size) / (1024.0 * 1024.0)
        : 0.0;
    HRESULT hr = SHEmptyRecycleBinA(
        nullptr,
        nullptr,
        SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND
    );

    cout << "-----------------------------------------------\n";
    if(SUCCEEDED(hr)){
        if(SUCCEEDED(qHr) && rbInfo.i64Size > 0) cout << "freed space: " << freedMB << " MB\n";
        cout << "Recycle Bin boost completed!\n";
    } else cerr << "error: failed to empty Recycle Bin (HRESULT: 0x" << hex << hr << dec << ").\n";
    cout << "-----------------------------------------------\n";
}

void cleanDir(const fs::path& path, const string& label) {
    if(path.empty()){
        cerr << "error: could not locate " << label << " directory.\n";
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

        bool isDir = entry.is_directory(ec);
        if(ec){
            ++skippedCount;
            ec.clear();
            continue;
        }

        if(!isDir){
            uintmax_t fileSize = entry.file_size(ec);
            if(ec){
                fileSize = 0;
                ec.clear();
            }
            allBytes += fileSize;

            fs::remove(entry.path(), ec);
            if(!ec){
                ++deletedCount;
                freedBytes += fileSize;
            } else {
                ++skippedCount;
                ec.clear();
            }
        } else {
            uintmax_t dirSize = 0;
            for(const auto& sub : fs::recursive_directory_iterator(entry.path(), fs::directory_options::skip_permission_denied, ec)){
                if(ec){
                    ec.clear();
                    continue;
                }
                if(sub.is_regular_file(ec) && !ec){
                    uintmax_t sz = sub.file_size(ec);
                    if (!ec) dirSize += sz;
                }
                ec.clear();
            }

            allBytes += dirSize;
            fs::remove_all(entry.path(), ec);
            if(!ec){
                ++deletedCount;
                freedBytes += dirSize;
            } else {
                ++skippedCount;
                ec.clear();
            }
        }
    }

    cout << "-----------------------------------------------\n";
    cout << "all items: " << allCount << "\n";
    cout << "all size before deletion: " << static_cast<double>(allBytes) / (1024.0 * 1024.0) << " MB\n";
    cout << "deleted items: " << deletedCount << "\n";
    cout << "skipped items: " << skippedCount << "\n";
    cout << "freed space:   " << static_cast<double>(freedBytes) / (1024.0 * 1024.0) << " MB\n";
    cout << label << " boost completed!\n";
    cout << "-----------------------------------------------\n";
}

void status(){
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    cout << "-----------------------------------------------\n";
    cout << "device status:\n";

    if(GlobalMemoryStatusEx(&memStatus)){
        double totalRAM = static_cast<double>(memStatus.ullTotalPhys) / (1024.0 * 1024.0 * 1024.0);
        double availRAM = static_cast<double>(memStatus.ullAvailPhys) / (1024.0 * 1024.0 * 1024.0);
        double usedRAM  = totalRAM - availRAM;

        cout << "RAM load:  " << memStatus.dwMemoryLoad << "%\n";
        cout << "RAM used:  " << usedRAM << " GB / " << totalRAM << " GB\n";
        cout << "RAM free:  " << availRAM << " GB\n";
    } else cerr << "error: could not get RAM status.\n";

    error_code ec;
    fs::space_info cSpace = fs::space("C:\\", ec);
    if(!ec){
        double totalC = static_cast<double>(cSpace.capacity) / (1024.0 * 1024.0 * 1024.0);
        double freeC  = static_cast<double>(cSpace.free) / (1024.0 * 1024.0 * 1024.0);
        double usedC  = totalC - freeC;

        cout << "C: total:  " << totalC << " GB\n";
        cout << "C: used:   " << usedC << " GB\n";
        cout << "C: free:   " << freeC << " GB\n";
    } else cerr << "error: could not get C: drive space.\n";
    cout << "-----------------------------------------------\n";
}

void showHelp(const string& arg, const string& secArg){
    if(arg.empty()){
        cout << "-----------------------------------------------\n";
        cout << "commands:\n";
        int idx = 1;
        for(const auto& c : commands){
            cout << idx++ << ". " << c.desc << "\n";
        }
        cout << "help <command>: show info about this command\n";
        cout << "-----------------------------------------------\n";
        return;
    }

    string target = secArg.empty() ? arg : (arg + " " + secArg);
    if(target == "temp") target = "boost temp";
    else if(target == "bin") target = "boost bin";
    else if(target == "cache") target = "boost cache";

    for(const auto& c : commands){
        if(target == c.name){
            cout << c.desc << "\n";
            return;
        }
    }
    cerr << "error: unknown command,\nuse \"help\" to check commands\n";
}

int main(){
    string inp;
    cout << "welcome, use \"help\" to check commands\n";
    while(true){
        cout << ">>> ";
        if(!getline(cin, inp)) break;
        stringstream ss(inp);
        string cmd, arg, secArg;
        ss >> cmd >> arg >> secArg;

        if(cmd.empty()) continue;
        else if(cmd == "exit") break;
        else if(cmd == "help") showHelp(arg, secArg);
        else if(cmd == "boost"){
            if(arg.empty()){
                cleanDir(getPath("TEMP"), "temp");
                cleanDir(getPath("cache"), "browser cache");
                cleanBin();
            } else if(arg == "temp") cleanDir(getPath("TEMP"), "temp");
            else if(arg == "bin") cleanBin();
            else if(arg == "cache") cleanDir(getPath("cache"), "cache");
            else cerr << "error: unknown command,\nuse \"help\" to check commands\n";
        }
        else if(cmd == "status") status();
        else cerr << "error: unknown command,\nuse \"help\" command to check commands\n";
    } return 0;
}