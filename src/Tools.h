#pragma once
#include <string>
#include <cmath>
#include <cstring>
#include "raylib.h"

bool mouseInRect(int posx, int posy, int width, int height);

bool mouseInCirc(int posx, int posy, int radius);

char* strToChar(std::string& str);

std::string numToStr(float num);

std::string intToStr(int num);

Color rainbowGen(int h);