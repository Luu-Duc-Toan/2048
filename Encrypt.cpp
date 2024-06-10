#include "Encrypt.h"

void Encrypt(string& data, string key)
{
	int dataSize = data.size();
	int keySize = key.size();
	int j = 0;
	for (int i = 0; i < dataSize; i++, j++) {
		if (j == keySize) j = 0;
		data[i] = data[i] ^ key[j];
	}
}
void Encrypt(long long& number, int& index, string key) {
	int keySize = key.size();
	string key8chars;
	for (int i = 0; i < 8; i++) {
		index++;
		if (index == keySize) index = 0;
		key8chars += key[index];
	}
	uint64_t ke64bits = 0;
	for (int i = 0; i < 8; ++i) {
		ke64bits |= (uint64_t)key8chars[i] << (56 - i * 8);
	}
	number = number ^ ke64bits;
}
void Encrypt(int& number, int& index, string key) {
	int keySize = key.size();
	string key8chars;
	for (int i = 0; i < 4; i++) {
		index++;
		if (index == keySize) index = 0;
		key8chars += key[index];
	}
	uint32_t ke64bits = 0;

	for (int i = 0; i < 4; ++i) {
		ke64bits |= (uint32_t)key8chars[i] << (56 - i * 8);
	}
	number = number ^ ke64bits;
}
void Decrypt(int& number, int& index, string key) {
	int keySize = key.size();
	string key4chars;
	for (int i = 0; i < 4; i++) {
		index++;
		if (index == keySize) index = 0;
		key4chars += key[index];
	}
	uint32_t ke64bits = 0;

	for (int i = 0; i < 4; ++i) {
		ke64bits |= (uint32_t)key4chars[i] << (56 - i * 8);
	}
	number = number ^ ke64bits;
}
void Decrypt(long long& number, int& index, string key) {
	int keySize = key.size();
	string key8chars;
	for (int i = 0; i < 8; i++) {
		index++;
		if (index == keySize) index = 0;
		key8chars += key[index];
	}
	uint64_t ke64bits = 0;
	for (int i = 0; i < 8; ++i) {
		ke64bits |= (uint64_t)key8chars[i] << (56 - i * 8);
	}
	number = number ^ ke64bits;
}
void Decrypt(string& data, string key)
{
	int dataSize = data.size();
	int keySize = key.size();
	int j = 0;
	for (int i = 0; i < dataSize; i++, j++) {
		if (j == keySize) j = 0;
		data[i] = data[i] ^ key[j];
	}
}