#include "AccountManager.h"

void AccountManager::DrawStopMessage() {
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawText("DO YOU REALLY WANT TO" , 60, 350, 50, BLACK);
	DrawText("STOP GAME T_T? (Y / N)" , 60, 400, 50, BLACK);
	EndDrawing();
}
void AccountManager::DrawWinWindow() {
	int helpFontSize = 30;
	int posY = 200 + helpFontSize * 2;
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawText("CONGRATULATIONS! YOU WON", (UI_SCREENSIZE - MeasureText("CONGRATULATIONS! YOU WON", helpFontSize)) / 2, posY, helpFontSize, WHITE);
	DrawText("DO YOU WANT TO KEEP PLAYING? (Y/N)", (UI_SCREENSIZE - MeasureText("DO YOU WANT TO KEEP PLAYING? (Y/N)", helpFontSize)) / 2, posY + helpFontSize, helpFontSize, WHITE);
	EndDrawing();
}
void AccountManager::DrawMaxScore() {
	long long bestScore;
	Color color = BLACK;
	if (toplist.size == 0 || toplist.userdatas[toplist.rank[0]].maxScore < currentAccount.gameBoard.score) {
		bestScore = currentAccount.gameBoard.score;
		color = RED;
	}
	else  bestScore = toplist.userdatas[toplist.rank[0]].maxScore;
	string bestScoreStr = ConvertScore(bestScore);
	DrawText("Best Score: ", 450, 20, 50, BLACK);
	DrawText(bestScoreStr.c_str(), 600 - MeasureText(bestScoreStr.c_str(), 50) / 2, 70, 50, color);
}
void AccountManager::Resume(int order) {
	string accountPassword = resumelist.GetAccountToResume(order);
	int spaceIndex = accountPassword.find_first_of(' ');
	string account = accountPassword.substr(0, spaceIndex);
	string password = accountPassword.substr(spaceIndex + 1);
	currentAccount.LoadResumeUser(account, password);
}
void AccountManager::Stop() 
{
	int key = 0;
	while (true) {
		DrawStopMessage();
		key = GetKeyPressed();
		if (key == 'Y') {
			toplist.Add(currentAccount);
			resumelist.EndGame(currentAccount);
			break;
		}
		else if (key == 'N') {
			return currentAccount.GameLoop();
		}
	}
}
void AccountManager::DrawLose() {
	int helpFontSize = 30;
	int helpPladding = 30;
	int posY = 290;
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawText("OOPS! YOU LOST", helpPladding, posY + helpFontSize, helpFontSize, WHITE);
	DrawText("EXIT:  PRESS 'E' (EXIT)", helpPladding, posY + 2 * helpFontSize, helpFontSize, WHITE);
	if (currentAccount.canUndoRedo)
		DrawText("KEEP PLAYING: PRESS 'U' (UNDO)", helpPladding, posY + 3 * helpFontSize, helpFontSize, WHITE);
	EndDrawing();
}
void AccountManager::Lose()
{
	toplist.Add(currentAccount);
	int key = 0;
	while (true) {
		DrawLose();
		key = GetKeyPressed();
		if (key == 0) continue;
		if (key == 'E') {
			return currentAccount.gameBoard.startMenuFunc();
		}
		else if (currentAccount.canUndoRedo) {
			if (key == 'U') {
				currentAccount.Undo();
				return currentAccount.GameLoop();
			}
		}
	}
}
void AccountManager::Win() {
	int key = 0;
	while (true) {
		DrawWinWindow();
		key = GetKeyPressed	();
		if (key == 'Y') {
			return currentAccount.GameLoop();
		}
		else if (key == 'N') {
			toplist.Add(currentAccount);
			return resumelist.EndGame(currentAccount);
		}
	}
}
bool AccountManager::CheckPassword(string account, string password) {
	if (toplist.CheckPassword(account, password)) return true;
	if (resumelist.CheckPassword(account, password)) return true;
	return false;
}
void AccountManager::NewGame(int boardSize, bool canUndoReodo, string account, string password) {
	currentAccount = User(boardSize, canUndoReodo, account, password);
	return currentAccount.NewGame();
}
bool AccountManager::Find(string account) {
	if (toplist.Find(account)) return true;
	if (resumelist.Find(account)) return true;
	return false;
}
void AccountManager::DrawTopList(int page) {
	toplist.Draw(page);
}
void AccountManager::DrawResumeList() {
	resumelist.Draw();
}
void AccountManager::Save() {
	toplist.Save();
	resumelist.Save();
}
AccountManager::~AccountManager() {
	toplist.Save();
	resumelist.Save();
}
AccountManager::AccountManager() {
	toplist.Load();	
	resumelist.Load();
	currentAccount.gameBoard.SetDrawBestScoreFunc(bind(&AccountManager::DrawMaxScore, this));
	currentAccount.gameBoard.SetStopFunc(bind(&AccountManager::Stop, this));
	currentAccount.gameBoard.SetLoseFunc(bind(&AccountManager::Lose, this));
	currentAccount.gameBoard.SetWinFunc(bind(&AccountManager::Win, this));
}