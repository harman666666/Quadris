#ifndef BOARD_H
#define BOARD_H

#include "Observer.h"
#include "Subject.h"
#include "Block.h"
#include "Cell.h"
#include "Display.h"
#include "BlockFactory.h"
#include "subscriptions.h"
#include "info.h"
#include <vector>

class Block;
//class Info;

class Board : public Observer, public Subject {
public:
	void currentBlockLeft(int n);
	void currentBlockRight(int n);
	void currentBlockDown(int n);
	void currentBlockRotateClockwise(int n);
	void currentBlockRotateCounterClockwise(int n);
	void currentBlockDrop();

	void clearBoard();
	void restart();

	Board(Display *d, int width=11, int height = 15, 
		std::string sequenceFile = "sequence.txt");

	Block generateBlock(); // maybe make a smart pointer

	Info getInfo() const; // Board is a subject...
	void notify(Subject &whoNotified); // ... and an observer
	SubscriptionType subType() const;

	std::vector<int> checkIfRowsComplete();
	std::vector<int> blockLevelsDeleted();

	void setLevel(int n);
	
	//Sets the current block to the one specified
	//name can be either be  
	void setCurrentBlock(std::string name);

	//Hint should be displayed on text by ??? 
	//and in Graphic Display by a block box
	void showHint();
	
	void noRandomBlock(std::string file);
	void restoreRandom();
	void setSeed(int seed);
	void setSequence(std::string sequenceFile);
	vector<int> clearRows(std::vector<int>);
	void clearRow(int row);

private:
	BlockFactory blockFactory;
	int currentLevel;

	std::vector< Cell > cells;
	std::vector< Block > blocks;
	Block nextBlock;
	Block currentBlock;

	const int width;
	const int height;

	bool canRotateLeft() const;
	bool canRotateRight() const;
	bool canMoveLeft() const;
	bool canMoveRight() const;
	bool canMoveDown();
	void getNextBlock();
	void updateScore() const;
	
};


#endif

