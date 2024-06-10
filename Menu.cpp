#include "Menu.h"
void Menu::DrawTitle() {
	Font font = LoadFont("Fonts/ARCADE.TTF");
	int fontSize = 300;
	Vector2 position = { (UI_SCREENSIZE - MeasureText("2048", fontSize) + fontSize / 2.5) / 2, 20 };
	DrawTextEx(font, "2048", position, fontSize, 1, YELLOW);
}
void Menu::DrawMenu(int option, bool canResume) {
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawTitle();
	int helpFontSize = 30;
	int posY = 200 + helpFontSize * 2;
	DrawText("MOVE: PRESS 'W' AND 'S'", 190, posY, helpFontSize, WHITE);
	DrawText("SELECT: PRESS 'ENTER'", 190, posY + helpFontSize, helpFontSize, WHITE);
	DrawText("EXIT: PRESS 'E'", 190, posY + helpFontSize * 2, helpFontSize, WHITE);
	int fontSize = 50;
	posY = posY + fontSize * 3.3;
	DrawText("NEW GAME", 220, posY, fontSize, BLACK);
	DrawText("GAME SETTINGS", 220, posY + fontSize * 1.3, fontSize, BLACK);
	DrawText("TOP-20 LIST", 220, posY + 2 * fontSize * 1.3, fontSize, BLACK);
	if (canResume) DrawText("RESUME", 220, posY + 3 * fontSize * 1.3, fontSize, BLACK);
	else  DrawText("RESUME", 220, posY + 3 * fontSize * 1.3, fontSize, ColorBrightness(BLACK, 0.8));
	DrawText(">>", 170, posY + option * fontSize * 1.3, fontSize, BLACK);
	EndDrawing();
}
void Menu::StartMenu() {
	int option = 0;
	bool canResume = false;
	if (accountManager.resumelist.size > 0)  canResume = true;
	int key = 0;
	while (true)
	{
		DrawMenu(option, canResume);
		key = GetKeyPressed();
		if (key == 0) continue;
		if (key == 'W') {
			option--;
			if (option == -1) {
				if (canResume) option = 3;
				else option = 2;
			}
		}
		else if (key == 'S') {
			option++;
			if ((canResume && option == 4) || (!canResume && option == 3)) option = 0;
		}
		else if (key == 257) { //enter	
			return StartNextWindow(option);
		}
		else if (key == 'E') {
			return;
		}
	}
}
void Menu::StartMenuWindow() {
	int option = 0;
	bool canResume = false;
	int key = 0;
	while (true)
	{
		DrawMenu(option, canResume);
		key = GetKeyPressed();
		if (key == 0) continue;
		if (key == 'W') {
			option--;
			if (option == -1) option = 3;
		}
		else if (key == 'S') {
			option++;
			if (option == 4) option = 0;
		}
		else if (key == 257) { //enter	
			return StartNextWindow(option);
		}
		else if (key == 'E') {
			return;
		}
	}
}
void Menu::ClearAccountPassword() {
	account = "";
	password = "";
}
void Menu::DrawNewGame(bool wrongPassword) {
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawTitle();
	int helpFontSize = 30;
	int posY = 200 + helpFontSize * 2;
	DrawText("15 CHARACTERS IS MAXIMUM", 60, posY, helpFontSize, WHITE);
	DrawText("ONLY CONTAINS LETTERS AND NUMBERS", 60, posY + helpFontSize, helpFontSize, WHITE);
	DrawText("DELETE: PRESS 'BACKSPACE'", 60, posY + helpFontSize * 2, helpFontSize, WHITE);
	DrawText("EXIT: DELETE ALL + PRESS 'BACKSPACE'", 60, posY + helpFontSize * 3, helpFontSize, WHITE);
	int fontSize = 30;
	posY = 480;
	DrawText("ACCOUNT: ", 110, posY, fontSize, BLACK);
	DrawText(account.c_str(), 280, posY, fontSize, BLACK);
	DrawText("PASSWORD: ", 110, posY + fontSize * 1.3, fontSize, BLACK);
	DrawText(password.c_str(), 300, posY + fontSize * 1.3, fontSize, BLACK);
	if (wrongPassword) DrawText("Wrong password.Please try again", 110, posY + 2 * fontSize * 1.3, fontSize, RED);
	EndDrawing();
}
void Menu::StartNewGameWindow() {
	ClearAccountPassword();
	int key = 0;
	bool wrongPass = false;
	bool getAccount = true;
	while (true) { //Get Account
		DrawNewGame(wrongPass);
		key = GetKeyPressed();
		if (key == 0) continue;
		if (ValidCharacterOfAccount(key)) {
			wrongPass = false;
			account += key;
			if (account.size() == 15) key = KEY_ENTER;
		}
		if (key == KEY_ENTER) {
			if (account == "") continue;
			while (true) { //get password
				DrawNewGame(wrongPass);
				key = GetKeyPressed();
				if (key == 0) continue;
				if (ValidCharacterOfAccount(key)) {
					cout << key;
					password += key;
					if (password.size() == 15) key = KEY_ENTER;
				}
				if (key == KEY_ENTER) {
					if (password == "") continue;
					if (accountManager.Find(account)) {
						if (!accountManager.CheckPassword(account, password)) {
							wrongPass = true;
							ClearAccountPassword();
							break;
						}
						else {
							accountManager.NewGame(boardSize, canUndoRedo, account, password);
							return;
						}
					}
					else {
						accountManager.NewGame(boardSize, canUndoRedo, account, password);
						return;
					}
				}
				else if (key == KEY_BACKSPACE) {
					if (password == "") {
						account.pop_back();
						break;
					}
					password.pop_back();
				}
			}
		}
		else if (key == KEY_BACKSPACE) {
			if (account == "") return StartMenu();
			account.pop_back();
		}
	}
}
bool Menu::ValidCharacterOfAccount(int key) {
	if ((key >= 'A' && key <= 'Z') ||
		(key >= '0' && key <= '9')) return true;
	return false;
}
void Menu::ChangeBoardSize(bool increase) {
	if (increase) {
		if (boardSize == maxBoardSize) boardSize = minBoardSize;
		else boardSize++;
	}
	else {
		if (boardSize == minBoardSize) boardSize = maxBoardSize;
		else boardSize--;
	}
}
void Menu::DrawSetting(int option, bool selectSize, bool selectUndoRedom) {
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawTitle();
	int helpFontSize = 30;
	int helpPladding = 150;
	int posY = 200 + helpFontSize * 2;
	if (!selectSize && !selectUndoRedom) {
		DrawText("MOVE: PRESS 'W' AND 'D'", helpPladding, posY, helpFontSize, WHITE);
		DrawText("SELECT: PRESS 'ENTER'", helpPladding, posY + helpFontSize, helpFontSize, WHITE);
	}
	else {
		DrawText("MODIFY: PRESS 'W' AND 'D'", helpPladding, posY, helpFontSize, WHITE);
		DrawText("UNSELECT: PRESS 'ENTER'", helpPladding, posY + helpFontSize, helpFontSize, WHITE);
	}
	DrawText("EXIT: PRESS 'E'", helpPladding, posY + helpFontSize * 2, helpFontSize, WHITE);
	int fontSize = 30;
	int contextPladding = 220;
	posY = 450;
	Color color = BLACK;
	DrawText("BOARD SIZE: ", contextPladding, posY, fontSize, BLACK);
	if (selectSize) color = BLUE;
	DrawText(TextFormat("%d x %d", boardSize, boardSize), contextPladding + 210, posY, fontSize, color);
	if (selectUndoRedom) color = BLUE;
	else color = BLACK;
	DrawText("UNDO & REDO: ", contextPladding, posY + fontSize * 1.3, fontSize, BLACK);
	if (canUndoRedo) DrawText("TRUE", contextPladding + 230, posY + fontSize * 1.3, fontSize, color);
	else DrawText("FALSE", contextPladding + 230, posY + fontSize * 1.3, fontSize, color);
	DrawText(">>", 170, posY + option * fontSize * 1.3, fontSize, BLACK);
	EndDrawing();
}
void Menu::StartSettingWindow() {
	int option = 0;
	bool selectSize = false;
	bool selectUndoRedo = false;
	int key = 0;
	while (true) {
		DrawSetting(option, selectSize, selectUndoRedo);
		key = GetKeyPressed();
		if (key == 0) continue;
		if (key == 'W') {
			if (selectSize) ChangeBoardSize(true);
			else if (selectUndoRedo) canUndoRedo = !canUndoRedo;
			else {
				option++;
				if (option == 2) option = 0;
			}
		}
		else if (key == 'S') {
			if (selectSize) ChangeBoardSize(false);
			else if (selectUndoRedo) canUndoRedo = !canUndoRedo;
			else {
				option--;
				if (option == -1) option = 1;
			}
		}
		else if (key == KEY_ENTER) {
			if (option == 0) selectSize = !selectSize;
			else selectUndoRedo = !selectUndoRedo;
		}
		else if (key == 'E') return StartMenu();
	}
}
void Menu::DrawTopList(int page) {
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawTitle();
	int helpFontSize = 30;
	int helpPladding = 160;
	int helpPosY = 200 + helpFontSize * 2;
	DrawText("EXIT: PRESS 'E'", helpPladding, helpPosY, helpFontSize, WHITE);

	int fontSize = 30;
	int contentPladding = 10;
	int contentPosY = 340;
	Color color = BLACK;
	int lastIndex = min(accountManager.toplist.size, 10 * (page + 1));
	string time;
	DrawText("        NAME                  TIME           SCORE", contentPladding, contentPosY, fontSize, YELLOW);
	for (int i = 10 * page; i < lastIndex; i++) {
		DrawText(TextFormat("%d. %s ", i + 1, accountManager.toplist.accounts[accountManager.toplist.rank[i]].c_str()), contentPladding, contentPosY + (i % 10 + 1) * fontSize * 1.3, fontSize, color);
		time = ConvertTime(accountManager.toplist.userdatas[accountManager.toplist.rank[i]].time);
		DrawText(time.c_str(), contentPladding + 380, contentPosY + (i % 10 + 1) * fontSize * 1.3, fontSize, color);
		string score = ConvertScore(accountManager.toplist.userdatas[accountManager.toplist.rank[i]].maxScore);
		DrawText(score.c_str(), 660 - MeasureText(score.c_str(), fontSize) / 2, contentPosY + (i % 10 + 1) * fontSize * 1.3, fontSize, color);
	}
	if (accountManager.toplist.size > 10 && page == 0) {
		DrawText("NEXT PAGE: PRESS 'D'", helpPladding, helpPosY + helpFontSize, helpFontSize, WHITE);
		DrawText(">>", UI_SCREENSIZE / 2, UI_SCREENSIZE - 40, fontSize, BLACK);
	}
	else if (accountManager.toplist.size > 10 && page == 1) {
		DrawText("PREVIOUS PAGE: PRESS 'A'", helpPladding, helpPosY + helpFontSize, helpFontSize, WHITE);
		DrawText("<<", UI_SCREENSIZE / 2, UI_SCREENSIZE - 40, fontSize, BLACK);
	}
	EndDrawing();
}
void Menu::StartTopListWindow() {
	int page = 0;
	while (!WindowShouldClose()) {
		char key = GetKeyPressed();
		if (key == 'E') {
			return StartMenu();
		}
		else if (accountManager.toplist.size > 10) {
			if (key == 'D' && page == 0) {
				page = 1;
			}
			else if (key == 'A' && page == 1) {
				page = 0;
			}
		}
		DrawTopList(page);
	}
}
void Menu::DrawResumeList(int option) {
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawTitle();
	int helpFontSize = 30;
	int helpPladding = 160;
	int helpPosY = 200 + helpFontSize * 2;
	DrawText("MOVE: PRESS 'W' AND 'S'", helpPladding, helpPosY, helpFontSize, WHITE);
	DrawText("SELECT: PRESS 'ENTER'", helpPladding, helpPosY + helpFontSize, helpFontSize, WHITE);
	DrawText("EXIT: PRESS 'E'", helpPladding, helpPosY + 2 * helpFontSize, helpFontSize, WHITE);
	int fontSize = 30;
	int contentPosY = 380;
	accountManager.DrawResumeList();
	if (option > -1) DrawText(">>", 40, contentPosY + (option + 1) * fontSize * 1.3, fontSize, BLACK);
	EndDrawing();
}
void Menu::StartResumeWindow() {
	int option = -1;
	if (accountManager.resumelist.size > 0) option = 0;
	int key = 0;
	while (true) {
		DrawResumeList(option);
		key = GetKeyPressed();
		if (key == 0) continue;
		if (key == 'W') {
			option--;
			if (option == -1) option = accountManager.resumelist.size - 1;
		}
		else if (key == 'S') {
			option++;
			if (option == accountManager.resumelist.size) option = 0;
		}
		else if (key == KEY_ENTER) {
			if (accountManager.resumelist.size < 0) continue;
			accountManager.Resume(option);
			return accountManager.currentAccount.GameLoop();
		}
		else if (key == 'E') {
			return StartMenu();
		}
	}
}
void Menu::StartNextWindow(int option) {
	if (option == 0) return StartNewGameWindow();
	else if (option == 1) return StartSettingWindow();
	else if (option == 2) return StartTopListWindow();
	else return StartResumeWindow();
}
Menu::Menu() {
	InitWindow(UI_SCREENSIZE, UI_SCREENSIZE, "2048");
	accountManager.currentAccount.gameBoard.SetStartMenuFunc(bind(&Menu::StartMenu, this));
}
