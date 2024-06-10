#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H
#include "Header.h"
#include "TopList.h"
#include "ResumeList.h"

struct AccountManager {
	User currentAccount;
	ResumeList resumelist;
	TopList toplist;
	
	void DrawStopMessage();
	void DrawWinWindow();
	void DrawLose();
	void Stop();
	void Lose();
	void Win();
	void DrawMaxScore();
	void Resume(int order);
	bool CheckPassword(string account, string password);
	void NewGame(int boardSize, bool canUndoReodo, string account, string password);
	bool Find(string account);
	void DrawTopList(int page);
	void DrawResumeList();
	void Save();
	~AccountManager();
	AccountManager();
};
#endif