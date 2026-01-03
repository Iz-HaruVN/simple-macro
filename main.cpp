#include <bits/stdc++.h>
#include <windows.h>
#include <random>
#include "json.hpp"
using json = nlohmann::json;

int parseKey(const std::string& s) {
    if (s == "left_click")
        return VK_LBUTTON;

    if (s == "right_click")
        return VK_RBUTTON;

    if (s.length() == 1) {
        return VkKeyScanA(s[0]) & 0xFF; // f -> VK_F
    }

    return -1; // invalid
}

int delay, random_delay;
int keybind;
int mhold_time;
int action_count = 0;
std::vector<int> keys;

void loadKeys(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Cannot open " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(f, line)) {
        // remove whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty()) continue;

        int vk = parseKey(line);
        if (vk == -1) {
            std::cerr << "Skipping invalid line: " << line << "\n";
            continue;
        }

        keys.push_back(vk);
    }

    std::cout << "Loaded " << keys.size() << " actions\n";
}

int randomInt(int min, int max){
    static std::mt19937 rng(std::random_device{}());
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

void pressKey(WORD key){
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void doLeftClick(){
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1,&Input,sizeof(INPUT));
    ZeroMemory(&Input,sizeof(INPUT));
    Sleep(mhold_time); // time between mouse down and up
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1,&Input,sizeof(INPUT));
}

void doRightClick(){
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1,&Input,sizeof(INPUT));
    ZeroMemory(&Input,sizeof(INPUT));
    Sleep(mhold_time); // time between mouse down and up
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1,&Input,sizeof(INPUT));
}

void run() {
    system("cls");
    int timer=0;
    action_count++;
    std::cout << "Macro run count: " << action_count << "\n";
    std::cout << "Macro started\n" ;

    for (int key : keys) {
        // ---- debug ----
        if (key == VK_LBUTTON)
            std::cout << "Processing: LEFT_CLICK\n";
        else if (key == VK_RBUTTON)
            std::cout << "Processing: RIGHT_CLICK\n";
        else
            std::cout << "Processing key VK: " << key << "\n";

        // ---- execute ----
        if (key == VK_LBUTTON) {
            doLeftClick();
            timer+=mhold_time;
        }
        else if (key == VK_RBUTTON) {
            doRightClick();
            timer+=mhold_time;
        }
        else {
            pressKey(key);
        }

        // ---- delay ----
        int total_delay = delay;
        if (random_delay > 0) {
            total_delay += randomInt(-random_delay, random_delay);
        }

        if (total_delay < 1)
            total_delay = 1;

        Sleep(total_delay);
        timer+=total_delay;
    }
    std::cout << "Macro finished in " << timer << " ms\n";
}

int main() {
    // ===== READ CONFIG FILE =====
    std::ifstream f("config.json");
    if (!f.is_open()) {
        std::cerr << "Cannot open config.json\n";
        system("pause");
        return 1;
    }

    json j;
    try {
        f >> j;
    } catch (...) {
        std::cerr << "JSON parse error\n";
        system("pause");
        return 1;
    }

    // ===== GET CONFIG VALUES =====
    delay = j.value("delay", 100);
    random_delay = j.value("random_delay", 0);

    // ---- keybind handling ----
    if (j["keybind"].is_number_integer()) {
        keybind = j["keybind"].get<int>();
    }
    else if (j["keybind"].is_string()) {
        std::string k = j["keybind"].get<std::string>();

        if (k.length() == 1) {
            keybind = VkKeyScanA(k[0]) & 0xFF; // "Q" -> VK_Q
        } else {
            std::cerr << "Invalid keybind string\n";
            system("pause");
            return 1;
        }
    }
    else {
        std::cerr << "Invalid keybind format\n";
        system("pause");
        return 1;
    }

    bool anti_loop = j.value("anti_loop", true);
    mhold_time = j.value("mouse_button_hold_time", 50);
    f.close();

    // ===== READ KEYS FILE =====
    loadKeys("keys.txt");
    if (keys.empty()) {
        std::cerr << "keys.txt is empty or invalid\n";
        system("pause");
        return 1;
    }

    std::cout << "Delay: " << delay << "\n";
    std::cout << "Random delay: " << random_delay << "\n";
    std::cout << "Keybind: " << keybind << "\n";
    std::cout << "Macro is running...\n";

    // ===== KEY LISTEN LOOP (ANTI-SPAM) =====
    bool pressed = false;

    while (true) {
        if (GetAsyncKeyState(keybind) & 0x8000) {
            if (anti_loop) {
                if (!pressed) {
                    run();
                    pressed = true; // mark as pressed
                }
            }
            else {
                run();
            }
        }
        else if (anti_loop) {
            pressed = false; // key released
        }

        Sleep(10); // reduce CPU usage
    }

    return 0;
}
