#include "Menu.h"
#define TESTCOMMENT

int main()
{
	Menu menu;
	menu.StartMenu();
}
 
#ifndef TESTCOMMENT
//TopList Test
int main() {
	AccountManager accountManager;
	//Add
	accountManager.toplist.AddTest("NAOT", "1234", 200, 942123);
	accountManager.toplist.AddTest("THANG", "1234", 200, 32231);
	accountManager.toplist.AddTest("LIREAL", "1234", 200, 5332);
	accountManager.toplist.AddTest("TTT", "1234", 200, 123);
	accountManager.toplist.AddTest("HUYCUTE", "1234", 200, 214511);
	accountManager.toplist.AddTest("CHUBBE", "1234", 200, 23411211);
	accountManager.toplist.AddTest("GOMEE", "1234", 200, 218394);
	accountManager.toplist.AddTest("GAIA", "1234", 200, 1917592);
	accountManager.toplist.AddTest("HORIZOH", "1234", 200, 97845);
	accountManager.toplist.AddTest("KKK", "1234", 200, 129843);
	accountManager.toplist.AddTest("MEANING", "1234", 200, 895378);
	//Update
	accountManager.toplist.AddTest("NAOT", "1234", 200, 99999999);
	cout << "Open TOP-20 on game to see the result";
}
#endif

#ifndef TESTCOMMENT
//ResumeList Test
int main() {
	User sampleUser1(4, true, "NAOT", "1234");
	User sampleUser2(5, true, "23120095", "1234");
	User sampleUser3(6, true, "LIREAL", "1234");
	User sampleUser4(7, true, "HUYCUTE", "1234");
	User sampleUser5(8, true, "HUYCUTE", "1234");
	AccountManager accountManager;
	accountManager.resumelist.Add(sampleUser1);
	accountManager.resumelist.Add(sampleUser2);
	accountManager.resumelist.Add(sampleUser3);
	accountManager.resumelist.Add(sampleUser4);
	accountManager.resumelist.Add(sampleUser5);
	cout << "Go to NEW GAME, create an account then EXIT to test MAX 5 ACCOUNT TO RESUME";
}
#endif

#ifndef TESTCOMMENT
//EncryptTest
int main() {
	string data = "HelloWorld";
	string key = "HelloWorld";
	//Test encrypt string
	cout << "Original string: " << data;
	Encrypt(data, key);
	cout << endl << "Encrypted string: " << data;
	Decrypt(data, key);
	cout << endl << "Decryoted string: " << data;
	//Test encrypt int
	int number = 0;
	int index = 0;
	cout << endl << "Original int: " << number;
	Encrypt(number, index, key);
	cout << endl << "Encrypted int: " << number;
	index = 0;
	Decrypt(number, index, key);
	cout << endl << "Decryoted int: " << number;
	//Test encrypt long long
	long long number2 = 1234128247;
	index = 0;
	cout << endl << "Original long long: " << number2;
	Encrypt(number2, index, key);
	cout << endl << "Encrypted long long: " << number2;
	index = 0;
	Decrypt(number2, index, key);
	cout << endl << "Decryoted long long: " << number2;
}
#endif

#ifndef TESTCOMMENT
 //Test Win: go to game loop, press "Q"
 //Test Lose: go to game loop, press "L"
 //Test Update best score: go to game loop, press "C" 
#endif

