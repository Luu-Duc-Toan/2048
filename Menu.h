#pragma once
#ifndef MENU_H
#define MENU_H
#include "Header.h"
#include "AccountManager.h"

struct Menu { 
	AccountManager accountManager;
	int maxResume = 0;
	bool canUndoRedo = true;
	int maxBoardSize = 10;
	int minBoardSize = 4;
	int boardSize = minBoardSize;
	string account = "";
	string password = "";

	void DrawTitle();
	void DrawMenu(int option, bool canResume);
	void StartMenu();
	void StartMenuWindow();
	void ClearAccountPassword();
	void DrawNewGame(bool wrongPassword);
	void StartNewGameWindow();
	bool ValidCharacterOfAccount(int key);
	void ChangeBoardSize(bool increase);
	void DrawSetting(int option, bool selectSize, bool selectUndoRedom);
	void StartSettingWindow();
	void DrawTopList(int page);
	void StartTopListWindow();
	void DrawResumeList(int option);
	void StartResumeWindow();
	void StartNextWindow(int option);
	Menu();
};
#endif