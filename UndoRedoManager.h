#pragma once
#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H
#include "Header.h"

#include "Stack.h"
#include "Gameboard.h"

struct UndoRedoManager
{
	Stack<Gameboard> undo;
	Stack<Gameboard> redo;
	Node<Gameboard>* current = NULL;

	~UndoRedoManager();
	void SetCurrent(Gameboard& board);
	void Move(bool isMoved, Gameboard& board);
	Gameboard Undo();
	Gameboard Redo();
};
#endif