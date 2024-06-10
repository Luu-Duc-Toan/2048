#pragma once
#ifndef STACKQUEUE_H
#define STACKQUEUE_H
#include "Header.h"

template <typename T> struct Node {
	Node<T>* next;
	T data;
	Node(T &data) {
		this->data = data;
		next = NULL;
	}
	~Node() {
		if (!next) return;
		delete next;
		next = NULL;
	}
};

template <typename T> struct Stack {
	Node<T> *head;
	int size;
	Stack() {
		head = NULL;
		size = 0;
	}
	~Stack() {
		if (isEmpty()) return;
		size = 0;
		delete head;
	}
	Node<T>* Top() {
		if (isEmpty()) return NULL;
		Node<T>* cur = head;
		head = head->next;
		cur->next = NULL;
		size--;
		return cur;
	}
	void Clear() { 
		if (!head) return;
		delete head;
		head = NULL;
		size = 0;
	}
	void Push(Node<T> *&node) {
		node->next = NULL;
		if (isEmpty()) {
			head = node;
		}
		else {
			node->next = head;
			head = node;
		}
		size++;
	}
	void Pop() {
		if (isEmpty()) return;
		head = head->next;
		size--;
	}
	bool isEmpty() {
		return head == NULL;
	}
}; 
#endif