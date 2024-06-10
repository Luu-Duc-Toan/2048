#pragma once
#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include "Header.h"
struct Gameboard { 
	int **board;
	int size;
	int hasWon = 0;
	bool canHorizontalMove = true;
	bool canVerticalMove = true;
	long long score = 0;
	function<void()> winFunc;
	function<void()> startMenuFunc;
	function<void()> loseFunc;
	function<void()> stopFunc;
	function<void()> drawBestScoreFunc;
	

	
	void WinTest();
	void LoseTest();
	void Win();
	void SetWinFunc(function<void()> func);
	void SetStartMenuFunc(function<void()> func);
	void SetLoseFunc(function<void()> func);
	void SetStopFunc(function<void()> func);
	void SetDrawBestScoreFunc(function<void()> func);
	Gameboard& operator=(const Gameboard& other);
	void SetBoardSize(int size);
	void New();
	void Reset();
	void DrawCellNumber(int i, int j, int recPosX, int recPosY, int cellSize, int maxFontSize);
	void Draw();
	void RandomCell();
	void MergeCell(int& currentCell, int& nextCell);
	void MoveCell(int& currentCell, int& blankCell);
	bool ContainZeroCell();
	bool ContainPairOfHorizontalCells();
	bool ContainPairOfVerticalCells();
	void CheckValidNextMove();
	bool MoveRight();
	bool MoveLeft();
	bool MoveDown();
	bool MoveUp();
	bool GameOver();
	Gameboard();
	Gameboard(int**& board, int size, bool canHorizontalMove, bool canVerticalMove, int score, bool hasWon);
	Gameboard(const Gameboard& gameBoard);
	~Gameboard();
};
#endif