#include "ResumeList.h"
ResumeList::ResumeList() {
	accounts = new string[prime];
	passwords = new string[prime];
	scores = new int[prime];
	times = new int[prime];
}
void ResumeList::Add(User &user){
	int originHash = Hash(user.account);
	int rehash = 0;
	int hash = originHash;
	while (accounts[hash] != "" && rehash < maxSize) {
		rehash++;
		hash = ReHash(originHash, rehash);
	}
	accounts[hash] = user.account;
	passwords[hash] = user.password;
	scores[hash] = user.gameBoard.score;
	times[hash] = user.timer.time;
	user.Save();
	user.Clear();
	size++;
}
int ResumeList::OrderOfAccount(string account) {
	int order = 0;
	for (int i = 0; i < prime; i++) {
		if (accounts[i] != "")
		{
			if (accounts[i] == account) break;
			order++;
		}
	}
	return order;
}
void ResumeList::Draw() {
	int fontSize = 30;
	int contentPladding = 10;
	int contentPosY = 380;
	int userIndex = 0;
	DrawText("            NAME               TIME           SCORE", contentPladding, contentPosY, fontSize, YELLOW);
	for (int i = 0; i < size; i++) {
		while (accounts[userIndex] == "") {
			userIndex++;
		}
		DrawText(TextFormat("      %d. %s ", i + 1, accounts[userIndex].c_str()), contentPladding, contentPosY + (i % 10 + 1) * fontSize * 1.3, fontSize, BLACK);
		DrawText(ConvertTime(times[userIndex]).c_str(), contentPladding + 400, contentPosY + (i + 1) * fontSize * 1.3, fontSize, BLACK);
		string score = ConvertScore(scores[userIndex]);
		DrawText(score.c_str(), 680 - MeasureText(score.c_str(), fontSize) / 2, contentPosY + (i + 1) * fontSize * 1.3, fontSize, BLACK);
		userIndex++;
	}
}
string ResumeList::GetAccountToResume(int order) {
	if(order == - 1) return "";
	int i = 0;
	for (i; i < prime; i++) {
		if (accounts[i] != "")
		{
			if (order == 0) break;
			order--;
		}
	}
	return accounts[i] + " " + passwords[i];
}
bool ResumeList::CheckPassword(string account, string password) {
	int originhash = Hash(account);
	int hash = originhash;
	for (int rehash = 0; rehash < maxSize; rehash++) {
		hash = ReHash(originhash, rehash);
		if (accounts[hash] == "") return false;
		if (accounts[hash] == account) {
			if (passwords[hash] == password) return true;
			return false;
		}
	}
	return false;
}
void ResumeList::DrawEndGame(int option, bool isChosen, bool alreadyInResumeList) {
	int helpFontSize = 30;
	int helpPladding = 40;
	int posY = 200;
	BeginDrawing();
	ClearBackground(BEIGE);
	if (alreadyInResumeList) {
		DrawText("YOU ALREADY SAVED THIS ACCOUNT", helpPladding, posY, helpFontSize, WHITE);
		DrawText("DO YOU WANT TO REPLACE IT?", helpPladding, posY + helpFontSize, helpFontSize, WHITE);
		DrawText("REPLACE: PRESS 'Y'", helpPladding, posY + 3 * helpFontSize, helpFontSize, WHITE);
		DrawText("KEEP: PRESS 'N'", helpPladding, posY + 4 * helpFontSize, helpFontSize, WHITE);
	}
	else {
		DrawText("YOU SAVED 5 ACCOUNT", helpPladding, posY, helpFontSize, WHITE);
		DrawText("PLEASE CHOOSE ONE TO REPLACE", helpPladding, posY + helpFontSize, helpFontSize, WHITE);
		DrawText("CANCEL: PRESS 'E'", helpPladding, posY + 2 * helpFontSize, helpFontSize, WHITE);
		if (isChosen)
			DrawText("REPLACE THIS ACCOUNT? (Y/N)", helpPladding, posY + 3 * helpFontSize, helpFontSize, WHITE);
	}
	Draw();
	int fontSize = 30;
	int contextPladding = 220;
	posY = 380;
	DrawText(">>", 20, posY + (option + 1) * fontSize * 1.3, fontSize, BLACK);
	EndDrawing();
}
void ResumeList::EndGame(User &user) {
	bool alreadyInResumeList = Find(user.account);
	int option = 0;
	if(alreadyInResumeList) option = OrderOfAccount(user.account);
	bool isChosen = false;
	int key = 0;
	if (size < maxSize && !alreadyInResumeList) {
		Add(user);
		return user.gameBoard.startMenuFunc();
	}
	while (true) {
		DrawEndGame(option, isChosen, alreadyInResumeList);
		key = GetKeyPressed();
		if (key == 0) continue;
		if (key == 'E') return user.gameBoard.startMenuFunc();
		else if (alreadyInResumeList) {
			if (key == 'Y') {
				Delete(option);
				Add(user);
				return user.gameBoard.startMenuFunc();
			}
			else if (key == 'N') {
				return user.gameBoard.startMenuFunc();
			}
		}
		else {
			if (isChosen) {
				if (key == 'Y') {
					Delete(option);
					Add(user);
					return user.gameBoard.startMenuFunc();
				}
				else if (key == 'N') {
					isChosen = false;
					continue;
				}
			}
			else if (key == 'W') {
				option--;
				if (option == -1) option = size - 1;
			}
			else if (key == 'S') {
				option++;
				if (option == size) option = 0;
			}
			else if (key == KEY_ENTER) {
				isChosen = true;
			}
		}
	}
}
void ResumeList::Delete(int order) {
	int i = 0;
	for (i; i < prime; i++) {
		if (accounts[i] != "")
		{
			if (order == 0) break;
			order--;
		}
	}
	string filename = "_Data/Users/" + accounts[i] + ".bin";
	remove(filename.c_str());
	accounts[i] = "";
	size--;
}
bool ResumeList::Find(string account) {
	int originHash = Hash(account);
	int hash = originHash;
	for (int rehash = 0; rehash < maxSize; rehash++) {
		hash = ReHash(originHash, rehash);
		if (accounts[hash] == "") return false;
		if (accounts[hash] == account) {
			return true;
		}
	}
	return false;
}
int ResumeList::Hash(string account) {
	int  hash = 0;
	for (char letter : account) {
		hash += letter;
	}
	return hash % maxSize;
}
int ResumeList::ReHash(int originHash, int rehashTimes) {
	return (originHash + (rehashTimes * rehashTimes) % prime) % prime;
}
void ResumeList::Save() {
	fstream file;
	file.open("_Data/ResumeList.bin", ios::binary | ios::out);
	file.write(reinterpret_cast<char*>(&size), sizeof(size));
	size_t length;
	for (int i = 0; i < prime; i++) {
		if (accounts[i] != "") {
			file.write(reinterpret_cast<char*>(&i), sizeof(i));
			length = accounts[i].size();
			file.write(reinterpret_cast<char*>(&length), sizeof(length));
			file.write(&accounts[i][0], length);
			length = passwords[i].size();
			file.write(reinterpret_cast<char*>(&length), sizeof(length));
			file.write(&passwords[i][0], length);
			file.write(reinterpret_cast<char*>(&scores[i]), sizeof(scores[i]));
			file.write(reinterpret_cast<char*>(&times[i]), sizeof(times[i]));
		}
	}
	file.close();
}
void ResumeList::Load() {
	fstream file;
	file.open("_Data/ResumeList.bin", ios::binary | ios::in);
	file.read(reinterpret_cast<char*>(&size), sizeof(size));
	size_t length;
	int index;
	for (int i = 0; i < size; i++) {
		file.read(reinterpret_cast<char*>(&index), sizeof(index));
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		accounts[index].resize(length);
		file.read(&accounts[index][0], length);
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		passwords[index].resize(length);
		file.read(&passwords[index][0], length);
		file.read(reinterpret_cast<char*>(&scores[index]), sizeof(scores[index]));
		file.read(reinterpret_cast<char*>(&times[index]), sizeof(times[index]));
	}
	file.close();
}
ResumeList::~ResumeList() {
	delete[] accounts;
	delete[] passwords;
	delete[] scores;
	delete[] times;
}