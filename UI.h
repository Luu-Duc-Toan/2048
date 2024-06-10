#pragma once
#ifndef UI_H
#define UI_H
#include "Header.h"
#define UI_ARROWLENGTH 5
#define UI_SCREENSIZE  800
#define UI_GAMEBOARDSIZE  600
#define UI_OFFSET  (UI_SCREENSIZE - UI_GAMEBOARDSIZE) / 2

string ConvertTime(int time);
string ConvertScore(long long score);
Color GetCellColor(int number);
#endif
