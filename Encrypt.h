#pragma once
#ifndef ENCRYPT_H
#define ENCRYPT_H
#include "Header.h"

void Encrypt(string& data, string key);
void Encrypt(long long& number, int& index, string key);
void Encrypt(int &number, int& index, string key);
void Decrypt(int &number, int& index, string key);
void Decrypt(long long& number, int& index, string key);
void Decrypt(string& data, string key);
#endif 
