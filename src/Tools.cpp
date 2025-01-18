#include "Tools.h"
#include "raylib.h"

bool mouseInRect(int posx, int posy, int width, int height) {
    int mx = GetMouseX(), my = GetMouseY();
    return (mx > posx && mx < posx+width) && (my > posy && my < posy+height);
}

bool mouseInCirc(int posx, int posy, int radius) {
    int mx = GetMouseX(), my = GetMouseY();
    float h = sqrt((posx-mx)*(posx-mx) + (posy-my)*(posy-my));
    return h < radius;
}

char* strToChar(std::string& str) {
    char* cstr = new char[str.length() + 1];

    for (int i = 0; i<(int)str.length(); i++) {
        cstr[i] = str[i];
    }
    cstr[str.length()] = '\0';

    return cstr;
}

// EJEMPLO DE PASAR STD::STR A CHAR*
// 
// std::string textStr = "...";
// char* textChar = strToChar(textStr);
// DrawText(textChar,...);
// delete[] textChar;

std::string numToStr(float num) {
    std::string str = std::to_string(num);
    return str;
}

std::string intToStr(int num) {
    std::string str = std::to_string(num);
    return str;
}

Color rainbowGen(int h) {
    h = h % 360;
    unsigned char r, g, b;
    float hh = h / 60.0f;

    float x = (1 - std::abs(std::fmod(hh, 2) - 1)) * 255;

    switch ((int)hh) {
        case 0:  r = 255; g =   x; b =   0; break;
        case 1:  r =   x; g = 255; b =   0; break;
        case 2:  r =   0; g = 255; b =   x; break;
        case 3:  r =   0; g =   x; b = 255; break;
        case 4:  r =   x; g =   0; b = 255; break;
        case 5:  r = 255; g =   0; b =   x; break;
        default: r =   0; g =   0; b =   0; break;
    }

    Color color = { r, g, b, 255 };
    return color;
}