#include "User.h"
User::User() {
	account = "";
	password = "";
}
void User::Resume() {
	gameBoard.Draw();
	timer.SetBegTime();
	return GameLoop();
}
User::User(int BoardSize, bool canUndoRedo, string account, string password) { 
	this->canUndoRedo = canUndoRedo;
	this->account = account;
	this->password = password;
	gameBoard.SetBoardSize(BoardSize);
	gameBoard.New();
	timer.Reset();
}
void User::NewGame() {
	gameBoard.New();
	if(canUndoRedo) 
		undoRedo.SetCurrent(gameBoard);
	timer.Reset();
	return GameLoop();
}
void User::UpdateTime() {
	timer.TimeElipsed();
	if (cntdown < 0) {
		cntdown = 2500;
	}
	else cntdown--;
}
void User::Move(bool isMoved) {
	if (!canUndoRedo) return;
	undoRedo.Move(isMoved, gameBoard);
}
void User::Undo() {
	if (!canUndoRedo) {
		while (true) {
			BeginDrawing();
			DrawRectangleRounded(Rectangle{ (UI_SCREENSIZE - 600) / 2,(UI_SCREENSIZE - 100) / 2, 600, 100 }, 0.12, 16, BROWN);
			int messageFontSize = 25;
			DrawText("Undo off (press any key to continue)",
				(UI_SCREENSIZE - MeasureText("Undo off (press any key to continue)", messageFontSize)) / 2,
				(UI_SCREENSIZE - messageFontSize) / 2,
				messageFontSize, BLACK);
			EndDrawing();
			if (GetKeyPressed() != 0) break;
		}
	}
	else if (undoRedo.undo.isEmpty()) {
		while (true) {
			BeginDrawing();
			DrawRectangleRounded(Rectangle{ (UI_SCREENSIZE - 600) / 2,(UI_SCREENSIZE - 100) / 2, 600, 100 }, 0.12, 16, BROWN);
			int messageFontSize = 25;
			DrawText("Can't Undo (press any key to continue)",
				(UI_SCREENSIZE - MeasureText("Can't Undo (press any key to continue)", messageFontSize)) / 2,
				(UI_SCREENSIZE - messageFontSize) / 2,
				messageFontSize, BLACK);
			EndDrawing();
			if (GetKeyPressed() != 0) break;
		}
	}
	else gameBoard = undoRedo.Undo();
}
void User::Redo() {
	if(!canUndoRedo) {
		while (true) {
			BeginDrawing();
			DrawRectangleRounded(Rectangle{ (UI_SCREENSIZE - 600) / 2,(UI_SCREENSIZE - 100) / 2, 600, 100 }, 0.12, 16, BROWN);
			int messageFontSize = 25;
			DrawText("Redo off (press any key to continue)",
				(UI_SCREENSIZE - MeasureText("Redo off (press any key to continue)", messageFontSize)) / 2,
				(UI_SCREENSIZE - messageFontSize) / 2,
				messageFontSize, BLACK);
			EndDrawing();
			if (GetKeyPressed() != 0) break;
		}
	}
	else if (undoRedo.redo.isEmpty()) {
		while (true) {
			BeginDrawing();
			DrawRectangleRounded(Rectangle{ (UI_SCREENSIZE - 600) / 2,(UI_SCREENSIZE - 100) / 2, 600, 100 }, 0.12, 16, BROWN);
			int messageFontSize = 25;
			DrawText("Can't Redo (press any key to continue)",
				(UI_SCREENSIZE - MeasureText("Can't Redo (press any key to continue)", messageFontSize)) / 2,
				(UI_SCREENSIZE - messageFontSize) / 2,
				messageFontSize, BLACK);
			if (GetKeyPressed() != 0) break;
			EndDrawing();
		}
	}
	else gameBoard = undoRedo.Redo();
}
void User::GameLoop() {
	int key = 0;
	while (true) {
		UpdateTime();
		Draw();
		if (gameBoard.GameOver()) {
			return gameBoard.loseFunc();
		}
		key = GetKeyPressed();
		if (key == 0) continue;
		if (key == 'W') {
			Move(gameBoard.MoveUp());
			if(gameBoard.hasWon == 1) return gameBoard.Win();
		}
		else if (key == 'S') {
			Move(gameBoard.MoveDown());
			if (gameBoard.hasWon == 1) return gameBoard.Win();
		}
		else if (key == 'A') {
			Move(gameBoard.MoveLeft());
			if (gameBoard.hasWon == 1) return gameBoard.Win();
		}
		else if (key == 'D') {
			Move(gameBoard.MoveRight());
			if (gameBoard.hasWon == 1) return gameBoard.Win();
		}
		else if (key == 'U') {
			Undo();
		}
		else if (key == 'R') {
			Redo();
		}
		else if (key == 'E') {
			return gameBoard.stopFunc();
		}
		else if (key == 'Q') {
			gameBoard.WinTest();
		}
		else if (key == 'L') {
			gameBoard.LoseTest();
		}
		else if (key == 'C') {
			CheatScore();
		}
	}
	Clear();
}
void User::Save() {
	fstream file;
	int index = 0;
	string nameFile = "_Data/Users/" + account + ".bin";
	file.open(nameFile, ios::binary | ios::out | ios::trunc);
	//Save Account
	file.write(reinterpret_cast<char*>(&canUndoRedo), sizeof(canUndoRedo));
	file.write(reinterpret_cast<char*>(&timer.time), sizeof(timer.time));
	//Save board
	int size_temp = gameBoard.size;
	Encrypt(size_temp, index, password);
	file.write(reinterpret_cast<char*>(&size_temp), sizeof(size_temp));
	file.write(reinterpret_cast<char*>(&gameBoard.canHorizontalMove), sizeof(gameBoard.canHorizontalMove));
	file.write(reinterpret_cast<char*>(&gameBoard.canVerticalMove), sizeof(gameBoard.canVerticalMove));
	Encrypt(gameBoard.score, index, password);
	file.write(reinterpret_cast<char*>(&gameBoard.score), sizeof(gameBoard.score));
	file.write(reinterpret_cast<char*>(&gameBoard.hasWon), sizeof(gameBoard.hasWon));
	for (int i = 0; i < gameBoard.size; i++) {
		for (int j = 0; j < gameBoard.size; j++) {
			Encrypt(gameBoard.board[i][j], index, password);
			file.write(reinterpret_cast<char*>(&gameBoard.board[i][j]), sizeof(gameBoard.board[i][j]));
		}
	}
	//Save undo
	int undoSize = undoRedo.undo.size;
	Encrypt(undoSize, index, password);
	file.write(reinterpret_cast<char*>(&undoSize), sizeof(undoSize));
	Node<Gameboard>* cur = undoRedo.undo.head;
	for (int i = 0; cur; i++) {
		file.write(reinterpret_cast<char*>(&cur->data.canHorizontalMove), sizeof(cur->data.canHorizontalMove));
		file.write(reinterpret_cast<char*>(&cur->data.canVerticalMove), sizeof(cur->data.canVerticalMove));
		Encrypt(cur->data.score, index, password);
		file.write(reinterpret_cast<char*>(&cur->data.score), sizeof(cur->data.score));
		file.write(reinterpret_cast<char*>(&cur->data.hasWon), sizeof(cur->data.hasWon));
		for (int j = 0; j < cur->data.size; j++) {
			for (int k = 0; k < cur->data.size; k++) {
				Encrypt(cur->data.board[j][k], index, password);
				file.write(reinterpret_cast<char*>(&cur->data.board[j][k]), sizeof(cur->data.board[j][k]));
			}
		}
		cur = cur->next;
	}
	//Save redo
	int redoSize  = undoRedo.redo.size;
	Encrypt(redoSize, index, password);
	file.write(reinterpret_cast<char*>(&redoSize), sizeof(redoSize));
	cur = undoRedo.redo.head;
	for (int i = 0; cur; i++) {
		file.write(reinterpret_cast<char*>(&cur->data.canHorizontalMove), sizeof(cur->data.canHorizontalMove));
		file.write(reinterpret_cast<char*>(&cur->data.canVerticalMove), sizeof(cur->data.canVerticalMove));
		Encrypt(cur->data.score, index, password);
		file.write(reinterpret_cast<char*>(&cur->data.score), sizeof(cur->data.score));
		file.write(reinterpret_cast<char*>(&cur->data.hasWon), sizeof(cur->data.hasWon));
		for (int j = 0; j < cur->data.size; j++) {
			for (int k = 0; k < cur->data.size; k++) {
				Encrypt(cur->data.board[j][k], index, password);
				file.write(reinterpret_cast<char*>(&cur->data.board[j][k]), sizeof(cur->data.board[j][k]));
			}
		}
		cur =  cur->next;
	}
	file.close();
}
void User::LoadResumeUser(string account, string password) {
	fstream file;
	string filename = "_Data/Users/" + account + ".bin";
	file.open(filename, ios::binary | ios::in);
	int index = 0;
	//Load Account
	this->account = account;
	this->password = password;
	file.read(reinterpret_cast<char*>(&canUndoRedo), sizeof(canUndoRedo));
	file.read(reinterpret_cast<char*>(&timer.time), sizeof(timer.time));
	//Load board
	int** tempBoard;
	int boardSize, hasWon;
	long long score;
	bool canHorizontalMove, canVerticalMove;
	file.read(reinterpret_cast<char*>(&boardSize), sizeof(boardSize));
	Decrypt(boardSize, index, password);
	file.read(reinterpret_cast<char*>(&canHorizontalMove), sizeof(canHorizontalMove));
	file.read(reinterpret_cast<char*>(&canVerticalMove), sizeof(canVerticalMove));
	file.read(reinterpret_cast<char*>(&score), sizeof(score));
	Decrypt(score, index, password);
	file.read(reinterpret_cast<char*>(&hasWon), sizeof(hasWon));
	tempBoard = new int* [boardSize];
	for (int i = 0; i < boardSize; i++) {
		*(tempBoard + i) = new int[boardSize];
	}
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			file.read(reinterpret_cast<char*>(&tempBoard[i][j]), sizeof(tempBoard[i][j]));
			Decrypt(tempBoard[i][j], index, password);
		}
	}
	this->gameBoard = Gameboard(tempBoard, boardSize, canHorizontalMove, canVerticalMove, score, hasWon);
	//Load undo
	Stack<Gameboard> tempStack;
	int sizeUndo;
	file.read(reinterpret_cast<char*>(&sizeUndo), sizeof(sizeUndo));
	Decrypt(sizeUndo, index, password);
	for (int k = 0; k < sizeUndo; k++) {
		file.read(reinterpret_cast<char*>(&canHorizontalMove), sizeof(canHorizontalMove));
		file.read(reinterpret_cast<char*>(&canVerticalMove), sizeof(canVerticalMove));
		file.read(reinterpret_cast<char*>(&score), sizeof(score));
		Decrypt(score, index, password);
		file.read(reinterpret_cast<char*>(&hasWon), sizeof(hasWon));
		tempBoard = new int* [boardSize];
		for (int i = 0; i < boardSize; i++) {
			*(tempBoard + i) = new int[boardSize];
		}
		for (int i = 0; i < boardSize; i++) {
			for (int j = 0; j < boardSize; j++) {
				file.read(reinterpret_cast<char*>(&tempBoard[i][j]), sizeof(tempBoard[i][j]));
				Decrypt(tempBoard[i][j], index, password);
			}
		}
		Gameboard tempGameBoard = Gameboard(tempBoard, boardSize, canHorizontalMove, canVerticalMove, score, hasWon);
		Node<Gameboard>* tempNode = new Node<Gameboard>(tempGameBoard);
		tempStack.Push(tempNode);
	}
	for (int i = 0; i < sizeUndo; i++) {
		Node<Gameboard>* tempNode = tempStack.Top();
		undoRedo.undo.Push(tempNode);
	}
	//Load redo
	int sizeRedo;
	file.read(reinterpret_cast<char*>(&sizeRedo), sizeof(sizeRedo));
	Decrypt(sizeRedo, index, password);
	for (int k = 0; k < sizeRedo; k++) {
		file.read(reinterpret_cast<char*>(&canHorizontalMove), sizeof(canHorizontalMove));
		file.read(reinterpret_cast<char*>(&canVerticalMove), sizeof(canVerticalMove));
		file.read(reinterpret_cast<char*>(&score), sizeof(score));
		Decrypt(score, index, password);
		file.read(reinterpret_cast<char*>(&hasWon), sizeof(hasWon));
		tempBoard = new int* [boardSize];
		for (int i = 0; i < boardSize; i++) {
			*(tempBoard + i) = new int[boardSize];
		}
		for (int i = 0; i < boardSize; i++) {
			for (int j = 0; j < boardSize; j++) {
				file.read(reinterpret_cast<char*>(&tempBoard[i][j]), sizeof(tempBoard[i][j]));
				Decrypt(tempBoard[i][j], index, password);
			}
		}
		Gameboard tempGameBoard = Gameboard(tempBoard, boardSize, canHorizontalMove, canVerticalMove, score, hasWon);
		Node<Gameboard>* tempNode = new Node<Gameboard>(tempGameBoard);
		tempStack.Push(tempNode);
	}
	for (int i = 0; i < sizeRedo; i++) {
		Node<Gameboard>* tempNode = tempStack.Top();
		undoRedo.redo.Push(tempNode);
	}
	file.close();
	//Set current
	if(canUndoRedo) undoRedo.SetCurrent(this->gameBoard);
	timer.SetBegTime();
}
void User::Clear() {
	account = "";
	password = "";
	if (!gameBoard.board) return;
	for (int i = 0; i < gameBoard.size; i++) {
		delete[] gameBoard.board[i];
	}
	delete[] gameBoard.board;
	gameBoard.board = NULL;
	//UndoRedo
	if (!canUndoRedo) return;
	if (undoRedo.current) delete undoRedo.current;
	undoRedo.current = NULL;
	undoRedo.undo.Clear();
	undoRedo.redo.Clear();
}
void User::CheatScore() {
	gameBoard.score += 50000000;
}
void User::Draw() {
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawText(TextFormat("Score: %s", ConvertScore(gameBoard.score).c_str()), 30, 20, 50, BLACK);
	DrawText(TextFormat("Time: %s", ConvertTime(timer.time).c_str()), 30, 80, 50, BLACK);
	gameBoard.drawBestScoreFunc();
	gameBoard.Draw();
	EndDrawing();
}