#include <Windows.h>
#include <iostream>

// just a boolean to check if it's good to cancel
bool cancelLeftMouseDown = false;

// timestamp of the last left mouse button down event
ULONGLONG lastLeftMouseDownTime = 0;

// This is just the double click time in ms
const DWORD doubleClickInterval = 80; // Adjust as needed

// mouse event callback function
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // make sure the event is called by mouse click
    if (nCode >= 0 && wParam == WM_LBUTTONDOWN) {
        // Get the current timestamp
        ULONGLONG currentTime = GetTickCount64();

        // the timestamp between new click and last click
        ULONGLONG timeDiff = currentTime - lastLeftMouseDownTime;

        // check if the timediff is < than the double click interval => it's double clicking => cancel
        if (timeDiff < doubleClickInterval) {
            return 1; // cancel the annoying and stupid event that keep closing spotify and my browser
        }

        // update old timestamp
        lastLeftMouseDownTime = currentTime;
    }

    // i searched this, and uh it's say: CallNextHookEx calls the next hook in the chain. This is what i know
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {   
    // print this to make user know ok this working
    std::cout << "This is working, the console will be disappeared." << std::endl;

    // apply hook
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // unhook it
    UnhookWindowsHookEx(mouseHook);

    return 0;
}
