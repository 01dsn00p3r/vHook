#include "spammer.h"

vector<string> spamMessages = {
    "THIS IS MY FIRST COMP!!! ^_^",
    "WOAH!!!",
    "hehe xD",
    "deez nartz",
    "sieg heil",
    "vlone.cc"
};


void MakeSpammer(float frameTime) {
    
    if (!pEngine->IsInGame())
        return;
    
    long currentTime_ms = GetEpochTime();
    static long timeStamp = currentTime_ms;
    
    if (currentTime_ms - timeStamp < 850)
        return;
    
    if (vars.misc.spammer) {
        
        if (spamMessages.empty())
            return;
        
        string message =spamMessages[rand() % spamMessages.size()];
        
        pstring str;
        str << "say" << " ";
        str << message;
        
        
        pEngine->ExecuteClientCmd(str.c_str());
    }
    
    timeStamp = currentTime_ms;
}
