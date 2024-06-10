#include "Gameboard.h"
void Gameboard::WinTest() {
	board[0][0] = 1024;
	board[0][1] = 1024;
	board[1][0] = 1024;
	hasWon = 0;
	CheckValidNextMove();
}
void Gameboard::LoseTest() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == 0 && j == 0) board[i][j] = 0;
			board[i][j] = pow(2, (i + 1) * (j + 1));
		}
	}
	CheckValidNextMove();
}
void Gameboard::Win() {
	hasWon = 2;
	winFunc();
}
void Gameboard::SetWinFunc(function<void()> func) {
	winFunc = func;
}
void Gameboard::SetStartMenuFunc(function<void()> func) {
	startMenuFunc = func;
}
void Gameboard::SetLoseFunc(function<void()> func) {
	loseFunc = func;
}
void Gameboard::SetStopFunc(function<void()> func) {
	stopFunc = func;
}
void Gameboard::SetDrawBestScoreFunc(function<void()> func) {
	drawBestScoreFunc = func;
}
Gameboard& Gameboard::operator=(const Gameboard& other)
{
	if (this == &other) {
		return *this;
	}
	if (board) {
		for (int i = 0; i < size; i++) {
			delete[] board[i];
		}
		delete[] board;
		board = NULL;
	}
	this->canHorizontalMove = other.canHorizontalMove;
	this->canVerticalMove = other.canVerticalMove;
	this->size = other.size;
	this->score = other.score;
	this->board = new int* [size];
	for (int i = 0; i < size; i++) {
		*(this->board + i) = new int[size];
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			this->board[i][j] = other.board[i][j];
		}
	}
	return *this;
}
void Gameboard::SetBoardSize(int size) {
	this->size = size;
	board = new int* [size];
	for (int i = 0; i < size; i++) {
		*(board + i) = new int[size];
	}
}
void Gameboard::New() {
	Reset();
	int randomNumberCount = (rand() % 2) + 1;
	for (int i = 0; i < randomNumberCount; i++) {
		RandomCell();
	}
}
void Gameboard::Reset() {
	score = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = 0;
		}
	}
	canHorizontalMove = true;
	canVerticalMove = true;
	hasWon = 0;
}
void Gameboard::DrawCellNumber(int i, int j, int recPosX, int recPosY, int cellSize, int maxFontSize) {
	int fontSize = maxFontSize;
	const char* numText = TextFormat("%d", board[i][j]);
	int numLength = MeasureText(numText, fontSize);
	while (numLength + 5 > cellSize) {
		fontSize -= 2;
		numLength = MeasureText(numText, fontSize);
	}
	int textPosX = recPosX + (cellSize - numLength) / 2;
	int textPosY = recPosY + (cellSize - fontSize) / 2;
	DrawText(numText, textPosX, textPosY, fontSize, BLACK);
}
void Gameboard::Draw() {
	int gap = 11 - size;
	int cellsize = (UI_GAMEBOARDSIZE - (size - 1) * gap) / size;
	int maxFontSize = cellsize / 2;
	int rectX;
	int rectY;
	for (int i = 0; i < size; i++) {
		rectY = UI_OFFSET + 60 + i * cellsize + i * gap;
		for (int j = 0; j < size; j++) {
			rectX = UI_OFFSET + j * cellsize + j * gap;
			DrawRectangle(rectX, rectY, cellsize, cellsize, GetCellColor(board[i][j]));
			DrawCellNumber(i, j, rectX, rectY, cellsize, maxFontSize);
		}
	}
}
void Gameboard::RandomCell() {
	int randRow = rand() % size;
	int randCol = rand() % size;
	while (board[randRow][randCol] != 0) {
		randRow = rand() % size;
		randCol = rand() % size;
	}
	board[randRow][randCol] = ((rand() % 2) + 1) * 2;
}
void Gameboard::MergeCell(int& currentCell, int& nextCell) {
	nextCell *= 2;
	currentCell = 0;
	if (nextCell == 2048 && hasWon == 0) hasWon = 1;
	score += nextCell;
}
void Gameboard::MoveCell(int& currentCell, int& blankCell) {
	blankCell = currentCell;
	currentCell = 0;
}
bool Gameboard::ContainZeroCell() {
	for (int currentRow = 0; currentRow < size; currentRow++) {
		for (int currentColumn = 0; currentColumn < size; currentColumn++) {
			if (board[currentRow][currentColumn] == 0) return true;
		}
	}
	return false;
}
bool Gameboard::ContainPairOfHorizontalCells() {
	for (int currentRow = 0; currentRow < size; currentRow++) {
		for (int currentColumn = 0; currentColumn < size - 1; currentColumn++) {
			if (board[currentRow][currentColumn] == board[currentRow][currentColumn + 1]) return true;
		}
	}
	return false;
}
bool Gameboard::ContainPairOfVerticalCells() {
	for (int currentRow = 0; currentRow < size - 1; currentRow++) {
		for (int currentColumn = 0; currentColumn < size; currentColumn++) {
			if (board[currentRow][currentColumn] == board[currentRow + 1][currentColumn]) return true;
		}
	}
	return false;
}
void Gameboard::CheckValidNextMove() {
	canHorizontalMove = true;
	canVerticalMove = true;
	if (ContainZeroCell()) return;
	if (!ContainPairOfHorizontalCells()) canHorizontalMove = false;
	if (!ContainPairOfVerticalCells()) canVerticalMove = false;
}
bool Gameboard::MoveRight() {
	if (!canHorizontalMove) return false;
	bool isChanged = false;
	bool* isMerged;//
	for (int currentRow = 0; currentRow < size; currentRow++) {
		isMerged = new bool[size];//
		fill_n(isMerged, size, false); //
		for (int currentColumn = size - 1; currentColumn >= 0; currentColumn--) {
			if (board[currentRow][currentColumn] == 0) continue;
			int currentColumntmp = currentColumn;
			for (int nextColumn = currentColumn + 1; nextColumn < size; nextColumn++) {
				if (board[currentRow][nextColumn] == 0) {
					MoveCell(board[currentRow][currentColumntmp], board[currentRow][nextColumn]);
					currentColumntmp = nextColumn;
					isChanged = true;
					continue;
				}
				if (board[currentRow][currentColumntmp] != board[currentRow][nextColumn]) break;
				if (isMerged[currentColumntmp]) break;//
				MergeCell(board[currentRow][currentColumntmp], board[currentRow][nextColumn]);
				isMerged[currentColumntmp] = true;//
				isChanged = true;
				break;
			}
		}
		delete isMerged; //
	}
	if (!isChanged) return false;
	RandomCell();
	CheckValidNextMove();
	return true;
}
bool Gameboard::MoveLeft() {
	if (!canHorizontalMove) return false;
	bool isChanged = false;
	bool* isMerged;
	for (int currentRow = 0; currentRow < size; currentRow++) {
		isMerged = new bool[size];//
		fill_n(isMerged, size, false);//
		for (int currentColumn = 0; currentColumn < size; currentColumn++) {
			if (board[currentRow][currentColumn] == 0) continue;
			int currentColumntmp = currentColumn;
			for (int nextColumn = currentColumn - 1; nextColumn >= 0; nextColumn--) {
				if (board[currentRow][nextColumn] == 0) {
					MoveCell(board[currentRow][currentColumntmp], board[currentRow][nextColumn]);
					currentColumntmp = nextColumn;
					isChanged = true;
					continue;
				}
				if (board[currentRow][currentColumntmp] != board[currentRow][nextColumn]) break;
				if (isMerged[currentColumntmp]) break;//
				MergeCell(board[currentRow][currentColumntmp], board[currentRow][nextColumn]);
				isMerged[currentColumntmp] = true;//
				isChanged = true;
				break;
			}
		}
		delete isMerged; //
	}
	if (!isChanged) return false;
	RandomCell();
	CheckValidNextMove();
	return true;
}
bool Gameboard::MoveDown() {
	if (!canVerticalMove) return false;
	bool isChanged = false;
	bool* isMerged;//
	for (int currentColumn = 0; currentColumn < size; currentColumn++) {
		isMerged = new bool[size];//
		fill_n(isMerged, size, false);//
		for (int currentRow = size - 1; currentRow >= 0; currentRow--) {
			if (board[currentRow][currentColumn] == 0) continue;
			int currentRowtmp = currentRow;
			for (int nextRow = currentRow + 1; nextRow < size; nextRow++) {
				if (board[nextRow][currentColumn] == 0) {
					MoveCell(board[currentRowtmp][currentColumn], board[nextRow][currentColumn]);
					currentRowtmp = nextRow;
					isChanged = true;
					continue;
				}
				if (board[currentRowtmp][currentColumn] != board[nextRow][currentColumn]) break;
				if (isMerged[currentRowtmp]) break;//
				MergeCell(board[currentRowtmp][currentColumn], board[nextRow][currentColumn]);
				isMerged[currentRowtmp] = true;//
				isChanged = true;
				break;
			}
		}
		delete isMerged; //
	}
	if (!isChanged) return false;
	RandomCell();
	CheckValidNextMove();
	return true;
}
bool Gameboard::MoveUp() {
	if (!canVerticalMove) return false;
	bool isChanged = false;
	bool* isMerged;//
	for (int currentColumn = 0; currentColumn < size; currentColumn++) {
		isMerged = new bool[size];//
		fill_n(isMerged, size, false);//
		for (int currentRow = 0; currentRow < size; currentRow++) {
			if (board[currentRow][currentColumn] == 0) continue;
			int currentRowtmp = currentRow;
			for (int nextRow = currentRow - 1; nextRow >= 0; nextRow--) {
				if (board[nextRow][currentColumn] == 0) {
					MoveCell(board[currentRowtmp][currentColumn], board[nextRow][currentColumn]);
					currentRowtmp = nextRow;
					isChanged = true;
					continue;
				}
				if (board[currentRowtmp][currentColumn] != board[nextRow][currentColumn]) break;
				if (isMerged[currentRowtmp]) break;//
				MergeCell(board[currentRowtmp][currentColumn], board[nextRow][currentColumn]);
				isMerged[currentRowtmp] = true;//
				isChanged = true;
				break;
			}
		}
		delete isMerged; //
	}
	if (!isChanged) return false;
	RandomCell();
	CheckValidNextMove();
	return true;
}
bool Gameboard::GameOver() {
	if (canHorizontalMove) return false;
	if (canVerticalMove) return false;
	return true;
}
Gameboard::Gameboard() {
	board = NULL;
}
Gameboard::Gameboard(int**& board, int size, bool canHorizontalMove, bool canVerticalMove, int score, bool hasWon) {
	this->board = board;
	this->size = size;
	this->canHorizontalMove = canHorizontalMove;
	this->canVerticalMove = canVerticalMove;
	this->score = score;
	this->hasWon = hasWon;
}
Gameboard::Gameboard(const Gameboard& gameBoard) {
	this->canHorizontalMove = gameBoard.canHorizontalMove;
	this->canVerticalMove = gameBoard.canVerticalMove;
	this->size = gameBoard.size;
	this->score = gameBoard.score;
	this->hasWon = gameBoard.hasWon;
	if (!gameBoard.board) {
		board = NULL;
		return;
	}
	this->board = new int* [size];
	for (int i = 0; i < size; i++) {
		*(this->board + i) = new int[size];
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			this->board[i][j] = gameBoard.board[i][j];
		}
	}
}
Gameboard::~Gameboard()
{
	if (!board) return;
	for (int i = 0; i < size; i++) {
		delete[] board[i];
	}
	delete[] board;
	board = NULL;
}