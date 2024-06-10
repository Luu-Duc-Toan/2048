#ifndef TOPLIST_H
#define TOPLIST_H
#include "Header.h"
#include "User.h"
struct TopUserData {
	string password;
	long long maxScore;
	double time;
	TopUserData();
	TopUserData(string password, double time, long long maxScore);
};
struct TopList {
	int size;
	int maxSize;
	int prime;
	string* accounts;
	TopUserData* userdatas;
	int* rank;
	int currentAccountRank;
	long long MinScore();
	double MinTime();
	void DrawCurrentUser(bool onList, bool addToList, long long maxScore, string account);
	void AddToRank(int index);
	void UpdateRank(int index);
	long long GetScore(string account);
	bool CheckPassword(string account, string password);
	void Draw(int page);
	void UpdateUser(string account, int time, long long newScore);
	void Add(User &user);
	void AddTest(string account, string password, int time, long long maxScore);
	void DeleteMinScoreUser();
	bool Find(string key);
	int Hash(string key);
	int ReHash(int originHash, int rehashTimes);
	void Save();
	void Load();
	TopList();
	~TopList();

};
#endif 
