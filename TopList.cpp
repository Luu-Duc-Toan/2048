#include "TopList.h"
TopUserData::TopUserData() {
	password = "";
	time = 0;
	maxScore = 0;
}
TopUserData::TopUserData(string password, double time, long long maxScore) {
	this->password = password;
	this->time = time;
	this->maxScore = maxScore;
}
TopList::TopList() {
	size = 0;
	maxSize = 20;
	prime = 43;
	currentAccountRank = -1;
	accounts = new string[prime];
	userdatas = new TopUserData[prime];
	rank = new int[maxSize];
}
void TopList::UpdateRank(int index) {
	for (int i = 0; i < size; i++) {
		if (rank[i] == index) {
			currentAccountRank = i;
			break;
		}
	}
	if (currentAccountRank == 0) return;
	int newRank = 0;
	for (int i = 0; i < size; i++) {
		if (userdatas[index].maxScore > userdatas[rank[i]].maxScore) {
			newRank = i;
			break;
		}
		else if (userdatas[index].maxScore == userdatas[rank[i]].maxScore) {
			if (userdatas[index].time < userdatas[rank[i]].maxScore) {
				newRank = i;
				break;
			}
		}
	}
	if (newRank == currentAccountRank) return;
	for (int i = currentAccountRank; i > newRank; i--) {
		rank[i] = rank[i - 1];
	}
	rank[newRank] = index;
	currentAccountRank = newRank;
}
long long TopList::GetScore(string account) {
	int originhash = Hash(account);
	int hash = originhash;
	for (int rehash = 0; rehash < maxSize; rehash++) {
		hash = ReHash(originhash, rehash);
		if (accounts[hash] == account)  return userdatas[hash].maxScore;
	}
	cout << "CHECK SCORE ERROR......................................................................." << endl;
	return -1;
}
void TopList::UpdateUser(string account, int time, long long newScore) {
	int originhash = Hash(account);
	int hash = originhash;
	for (int rehash = 0; rehash < maxSize; rehash++) {
		hash = ReHash(originhash, rehash);
		if (accounts[hash] == account) {
			if (newScore > userdatas[hash].maxScore) {
				userdatas[hash].maxScore = newScore;
				userdatas[hash].time = time;
				UpdateRank(hash);
			}
			return;
		}
	}
}
void TopList::AddTest(string account, string password, int time, long long maxScore) {
	bool onList = Find(account);
	bool addToList = false;
	if (onList) {
		long long pastScore = GetScore(account);
		if (maxScore > pastScore) {
			UpdateUser(account, time, maxScore);
			addToList = true;
			return;
		}
	}
	else if (size < maxSize) addToList = true;
	else if (size == maxSize) {
		if (maxScore > MinScore() || (maxScore == MinScore() && time < userdatas[rank[size - 1]].time)) {
			DeleteMinScoreUser();
			addToList = true;
		}
	}
	if (addToList) {
		int originHash = Hash(account);
		int rehash = 0;
		int hash = originHash;
		while (accounts[hash] != "" && rehash < maxSize) {
			if (accounts[hash] == account) break;
			rehash++;
			hash = ReHash(originHash, rehash);
		}
		if (rehash < maxSize) {
			onList = true;
			accounts[hash] = account;
			TopUserData user(password, time, maxScore);
			userdatas[hash] = user;
			AddToRank(hash);
			size++;
		}
	}
	return;
}
long long TopList::MinScore() {
	if (size == 0) return 0;
	return userdatas[rank[size - 1]].maxScore;
}
double TopList::MinTime() {
	if (size == 0) return 0;
	return userdatas[rank[size - 1]].time;
}
void TopList::Draw(int page) {
	int fontSize = 30;
	int contentPladding = 10;
	int contentPosY = 340;
	Color color = BLACK;
	int lastIndex = min(size, 10 * (page + 1));
	string time;
	DrawText("        NAME                  TIME           SCORE", contentPladding, contentPosY, fontSize, YELLOW);
	for (int i = 10 * page; i < lastIndex; i++) {
		DrawText(TextFormat("%d. %s ", i + 1, accounts[rank[i]].c_str()), contentPladding, contentPosY + (i % 10 + 1) * fontSize * 1.3, fontSize, color);
		time = ConvertTime(userdatas[rank[i]].time);
		DrawText(time.c_str(), contentPladding + 380, contentPosY + (i % 10 + 1) * fontSize * 1.3, fontSize, color);
		string score = ConvertScore(userdatas[rank[i]].maxScore);
		DrawText(score.c_str(), 660 - MeasureText(score.c_str(), fontSize) / 2, contentPosY + (i % 10 + 1) * fontSize * 1.3, fontSize, color);
	}
}
void TopList::DrawCurrentUser(bool onList, bool addToList, long long maxScore, string account)
{
	int helpFontSize = 30;
	int fontSize = 30;
	int contentPladding = 30;
	int contentPosY = 340;
	int posY = contentPosY - 50;
	Color color = BLACK;
	BeginDrawing();
	ClearBackground(BEIGE);
	DrawText("PRESS ANY KEY TO CONTINUE", contentPladding, posY, helpFontSize, WHITE);

	if (addToList) {
		DrawText("YOUR RESULTS: ", contentPladding, posY + helpFontSize, helpFontSize, YELLOW);
		DrawText("        NAME                  TIME           SCORE", contentPladding, contentPosY + fontSize * 1.3, fontSize, YELLOW);
		DrawText(TextFormat("%d. %s ", currentAccountRank + 1, accounts[rank[currentAccountRank]].c_str()), contentPladding, contentPosY +  2 * fontSize * 1.3, fontSize, color);
		string time = ConvertTime(userdatas[rank[currentAccountRank]].time);
		DrawText(time.c_str(), contentPladding + 380, contentPosY + 2 * fontSize * 1.3, fontSize, color);
		string score = ConvertScore(userdatas[rank[currentAccountRank]].maxScore);
		DrawText(score.c_str(), 690 - MeasureText(score.c_str(), fontSize) / 2, contentPosY + 2 * fontSize * 1.3, fontSize, color);
	}
	else if (onList) {
		DrawText(TextFormat("CURRENT SCORE: %s", ConvertScore(maxScore).c_str()), contentPladding, 370, 30, YELLOW);
		DrawText(TextFormat("YOU'VE PLAYED AN HIGHER RESULT: %s", ConvertScore(GetScore(account)).c_str()), contentPladding, 400, 30, YELLOW);
	}
	else {
		DrawText(TextFormat("GET AT LEAST %s MORE TO BE ON TOP LIST", ConvertScore(MinScore() - maxScore).c_str()), 10, 375, 30, BLACK);
	}
	EndDrawing();
}
void TopList::AddToRank(int index) {
	if (size == 0) {
		rank[0] = index;
		currentAccountRank = 0;
	}
	else if (size == 1) {
		bool larger = false;
		if (userdatas[index].maxScore > userdatas[rank[0]].maxScore) 
			larger = true;
		else if (userdatas[index].maxScore == userdatas[rank[0]].maxScore)
		{
			if (userdatas[index].time < userdatas[rank[0]].time) larger = true;
		}
		if (larger) {
			rank[1] = rank[0];
			rank[0] = index;
			currentAccountRank = 0;
		}
		else {
			rank[1] = index;
			currentAccountRank = 1;
		}
	}
	else {
		int l = 0;
		int r = size - 1;
		if (userdatas[index].maxScore > userdatas[rank[0]].maxScore) {
			for (int i = size; i > 0; i--) {
				rank[i] = rank[i - 1];
			}
			rank[0] = index;
			currentAccountRank = 0;
		}
		else if (userdatas[index].maxScore < userdatas[rank[r]].maxScore) {
			rank[size] = index;
			currentAccountRank = size;
		}
		else {
			int mid = (r + l) / 2;
			while (l < r) {
				mid = (r + l) / 2;
				if (userdatas[index].maxScore == userdatas[rank[mid]].maxScore) break;
				else if (userdatas[index].maxScore > userdatas[rank[mid]].maxScore) {
					r = mid;
				}
				else {
					l = mid + 1;
				}
			}
			mid = (r + l) / 2;
			while (mid < size) {
				if (userdatas[index].maxScore == userdatas[rank[mid]].maxScore) {
					if (userdatas[index].time > userdatas[rank[mid]].time) {
						mid++;
					}
					else break;
				}
				else break;
			}
			for (int i = size; i > mid; i--) {
				rank[i] = rank[i - 1];
			}
			rank[mid] = index;
			currentAccountRank = mid;
		}
	}
}
bool TopList::CheckPassword(string account, string password) {
	int originhash = Hash(account);
	int hash = originhash;
	for (int rehash = 0; rehash < maxSize; rehash++) {
		hash = ReHash(originhash, rehash);
		if (accounts[hash] == "") return false;
		if (accounts[hash] == account) {
			if (userdatas[hash].password == password) return true;
			return false;
		}
	}
	return false;
}
void TopList::Add(User &user) {
	bool onList = Find(user.account);
	bool addToList = false;
	if (onList) {
		long long pastScore = GetScore(user.account);
		if(user.gameBoard.score > pastScore) {
			UpdateUser(user.account, user.timer.time, user.gameBoard.score);
			addToList = true;
			int key = 0;
			while (true) {
				DrawCurrentUser(onList, addToList, user.gameBoard.score, user.account);
				key = GetKeyPressed();
				if (key != 0) return;
			}
		}
	}
	else if (size < maxSize) addToList = true;
	else if (size == maxSize) {
		if (user.gameBoard.score > MinScore() || 
			(user.gameBoard.score == MinScore() && user.timer.time < userdatas[rank[size - 1]].time)) {
			DeleteMinScoreUser();
			addToList = true;
		}
	}
	if (addToList) {
		int originHash = Hash(user.account);
		int rehash = 0;
		int hash = originHash;
		while (accounts[hash] != "" && rehash < maxSize) {
			if (accounts[hash] == user.account) break;
			rehash++;
			hash = ReHash(originHash, rehash);
		}
		if (rehash < maxSize) {
			onList = true;
			accounts[hash] = user.account;
			TopUserData userData(user.password, user.timer.time, user.gameBoard.score);
			userdatas[hash] = userData;
			AddToRank(hash);
			size++;
		}
	}
	int key = 0;
	while (true) {
		DrawCurrentUser(onList, addToList, user.gameBoard.score, user.account);
		key = GetKeyPressed();
		if (key != 0) return;
	}
}
void TopList::DeleteMinScoreUser() {
	accounts[rank[size - 1]] = "";
	size--;
}
bool TopList::Find(string key) {
	int originHash = Hash(key);
	int hash = originHash;
	for (int rehash = 0; rehash < maxSize; rehash++) {
		hash = ReHash(originHash, rehash);
		if (accounts[hash] == "") return false;
		if (accounts[hash] == key) {
			return true;
		}
	}
	return false;
}
int TopList::Hash(string key) {
	int  hash = 0;
	for (char letter : key) {
		hash += letter;
	}
	return hash % 20;
}
int TopList::ReHash(int originHash, int rehashTimes) {
	return (originHash + (rehashTimes * rehashTimes) % prime) % prime;
}
void TopList::Save() {
	fstream file;
	file.open("_Data/TopList.bin", ios::out | ios::binary);
	file.write(reinterpret_cast<const char*>(&maxSize), sizeof(maxSize));
	file.write(reinterpret_cast<const char*>(&size), sizeof(size));
	size_t length;
	for (int i = 0; i < size; i++) {
		file.write(reinterpret_cast<const char*>(&rank[i]), sizeof(rank[i]));
		length = accounts[rank[i]].size();
		file.write(reinterpret_cast<const char*>(&length), sizeof(length));
		file.write(accounts[rank[i]].c_str(), length);
		length = userdatas[rank[i]].password.size();
		file.write(reinterpret_cast<const char*>(&length), sizeof(length));
		file.write(userdatas[rank[i]].password.c_str(), length);
		file.write(reinterpret_cast<const char*>(&userdatas[rank[i]].time), sizeof(userdatas[rank[i]].time));
		int index = 0;
		Encrypt(userdatas[rank[i]].maxScore, index, userdatas[rank[i]].password);
		file.write(reinterpret_cast<const char*>(&userdatas[rank[i]].maxScore), sizeof(userdatas[rank[i]].maxScore));
	}
	file.close();
}
void TopList::Load() {
	fstream file;
	file.open("_Data/TopList.bin", ios::in | ios::binary);
	file.read(reinterpret_cast<char*>(&maxSize), sizeof(maxSize));
	file.read(reinterpret_cast<char*>(&size), sizeof(size));
	size_t length;
	for (int i = 0; i < size; i++) {
		file.read(reinterpret_cast<char*>(&rank[i]), sizeof(rank[i]));
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		accounts[rank[i]].resize(length);
		file.read(&accounts[rank[i]][0], length);
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		userdatas[rank[i]].password.resize(length);
		file.read(&userdatas[rank[i]].password[0], length);
		file.read(reinterpret_cast<char*>(&userdatas[rank[i]].time), sizeof(userdatas[rank[i]].time));
		file.read(reinterpret_cast<char*>(&userdatas[rank[i]].maxScore), sizeof(userdatas[rank[i]].maxScore));
		int index = 0;
		Decrypt(userdatas[rank[i]].maxScore, index, userdatas[rank[i]].password);
	}
	file.close();
}
TopList::~TopList() {
	delete[] accounts;
	delete[] userdatas;
	delete[] rank;
}