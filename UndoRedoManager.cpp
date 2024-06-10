#include "UndoRedoManager.h"

UndoRedoManager::~UndoRedoManager() {
	if (current) delete current;
	current = NULL;
	undo.Clear();
	redo.Clear();
}
void UndoRedoManager::SetCurrent(Gameboard& board)
{
	current = new Node<Gameboard>(board);
}
void UndoRedoManager::Move(bool isMoved, Gameboard& board) {
	if (!isMoved) return;
	if (!redo.isEmpty()) redo.Clear();
	undo.Push(current);
	current = new Node<Gameboard>(board);
}
Gameboard UndoRedoManager::Undo()
{
	redo.Push(current);
	current = undo.Top();
	return current->data;
}
Gameboard UndoRedoManager::Redo()
{
	undo.Push(current);
	current = redo.Top();
	return current->data;
}
