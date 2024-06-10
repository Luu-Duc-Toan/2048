#ifndef RESUMELIST_H
#define RESUMELIST_H
#include "Header.h"
#include "User.h"

struct ResumeList {
	string *accounts;
	string *passwords;
	int *scores;
	int *times;
	int size = 0;
	int maxSize = 5;
	int prime = 13;
	ResumeList();
	void Add(User &user);
	void Draw();
	string GetAccountToResume(int order);
	bool CheckPassword(string account, string password);
	void EndGame(User &user);
	void Delete(int order);
	int OrderOfAccount(string account);
	void DrawEndGame(int option, bool isChosen, bool alreadyInResumeList);
	bool Find(string account);
	int Hash(string account);
	int ReHash(int originHash, int rehashTimes);
	void Save();
	void Load();
	~ResumeList();
};
#endif
