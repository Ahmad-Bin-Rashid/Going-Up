#include <iostream>
#include <windows.h>

void detectMouseEvents() {
    while (true) {
        // Get the current state of the mouse buttons
        SHORT leftButton = GetAsyncKeyState(VK_LBUTTON);  // Left Mouse Button
        SHORT rightButton = GetAsyncKeyState(VK_RBUTTON); // Right Mouse Button
        
        // Get the current mouse position
        POINT pt;
        if (GetCursorPos(&pt)) {
            std::cout << "Mouse Position: (" << pt.x << ", " << pt.y << ")\n";
        }

        // Check if the left mouse button is pressed
        if (leftButton & 0x8000) {
            std::cout << "Left Mouse Button Pressed.\n";
        }

        // Check if the right mouse button is pressed
        if (rightButton & 0x8000) {
            std::cout << "Right Mouse Button Pressed.\n";
        }

        // Sleep for a bit to avoid excessive CPU usage
        Sleep(100);
    }
}

int main() {
    std::cout << "Detecting mouse events...\n";
    detectMouseEvents();
    return 0;
}