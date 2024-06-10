#include "UI.h"

string ConvertScore(long long score) {
	if (score < 1000000) return to_string(score);
	score /= 1000000;
	if (score < 1000) return to_string(score) + " (M)";
	score /= 1000;
	if (score < 1000) return to_string(score) + " (B)";
	score /= 1000;
	if (score < 1000) return to_string(score) + " (T)";
	score /= 1000;
	return to_string(score) + " (Q)";
}
string ConvertTime(int time) {
    string timeStr = "";
    int hour = time / 3600;
    int minute = (time % 3600) / 60;
    int second = time % 60;
    if (hour < 10) timeStr += '0';
    timeStr += to_string(hour);
    timeStr += ':';
    if (minute < 10) timeStr += '0';
    timeStr += to_string(minute);
    timeStr += ':';
    if (second < 10) timeStr += '0';
    timeStr += to_string(second);
    return timeStr;
}
Color GetCellColor(int number) {
	if (number == 0) return ColorBrightness(LIGHTGRAY, 0.9);
	else if (number == 2) return ColorBrightness(YELLOW, 0.7);
	else if (number == 4) return ColorBrightness(BEIGE, 0.3);
	else if (number == 8) return ColorBrightness(BEIGE, -0.1);
	else if (number == 16) return ColorBrightness(BLUE, 0.5);
	else if (number == 32) return ColorBrightness(BLUE, 0.2);
	else if (number == 64) return ColorBrightness(YELLOW, 0.2);
	else if (number == 128) return ColorBrightness(ORANGE, 0.3);
	else if (number == 256) return ORANGE;
	else if (number == 512) return ColorBrightness(GREEN, 0.2);
	else if (number == 1024) return ColorBrightness(DARKPURPLE, 0.4);
	else if (number == 2048) return DARKPURPLE;
	else return ColorBrightness(GetCellColor(number / 1024), -0.6);
}
