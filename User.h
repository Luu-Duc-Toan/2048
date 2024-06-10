#ifndef USER_H
#define USER_H
#include "Header.h"
#include "UndoRedoManager.h"
struct User {
	Gameboard gameBoard;
	string account;
	string password;
	UndoRedoManager undoRedo;
	Timer timer;
	int cntdown = 2500;
	bool canUndoRedo;
	
	void Clear();
	void CheatScore();
	void Resume();
	void NewGame();
	void UpdateTime();
	void Move(bool isMoved);
	void Undo();
	void Redo();
	void Draw();
	void GameLoop();
	void Save();
	void LoadResumeUser(string account, string password);
	User();
	User(int BoardSize, bool canUndoRedo, string account, string password);
};
#endif 
