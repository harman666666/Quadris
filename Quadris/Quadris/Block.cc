#include "Block.h"
#include "subscriptions.h"
#include "info.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
using namespace std;


// NOTE: height and width start at zero (ie. I-block has height 0)
int calcHeight(std::vector < std::pair < int, int > > coords) {
	int highest = 0;
	for (auto i:coords) {
		if (i.first > highest) highest = i.first;
	}
	return highest;
}

int calcWidth(std::vector < std::pair < int, int > > coords) {
	int highest = 0;
	for (auto i:coords) {
		if (i.second > highest) highest = i.second;
	}
	return highest;
}

pair<int, int> Block::findLowest() {
	int furthestLeft = cells[0].col;
	int furthestDown = cells[0].row;
	for (auto i:cells) {
		if (i.col < furthestLeft) furthestLeft = i.col;
		if (i.row > furthestDown) furthestDown = i.row;
	}
	pair<int,int> p2{furthestDown, furthestLeft};
	return p2;
}

//This is for the block parser
Block::Block(char dispChar,
	 	  std::string colour,
		  std::string name,
		  std::vector < std::pair < int, int > > coords
		  ): name(name), colour(colour), dispChar(dispChar), coords(coords) {

	for (auto i:coords) {
		cells.emplace_back(Cell{this, dispChar, i.first+3, i.second});
	} // Add 3 to le height for safety purposes

	height = calcHeight(coords);
	width = calcWidth(coords);

	lowerLeft = findLowest();

	notifyObservers(SubscriptionType::blockChange);
}

Block::Block() {} // default ctor

void Block::rotateUpdate() {
	int csize = cells.size();
	for (int i=0; i < csize; i++) { // cannot use auto here
		cells[i].row = lowerLeft.first -width + coords[i].first;
		cells[i].col = lowerLeft.second + coords[i].second;
		cout << lowerLeft.first << endl;
	}
	lowerLeft = findLowest();
	height = calcHeight(coords);
	width = calcWidth(coords);
	notifyObservers(SubscriptionType::blockChange);
}

void Block::rotateClockWise(int restraint) {
	if (lowerLeft.second + height >= restraint) return;
	prevCells = cells;
	for (auto &i:coords) {
		int row = i.first;
		int col = i.second;
		i.first = col;
		i.second = height - row;
	}
	rotateUpdate();
}

void Block::clearBlockFromScreen(){
	prevCells = cells;
	cells.clear();
	notifyObservers(SubscriptionType::blockChange);
}

void Block::rotateCounterClockWise(int restraint) {
	if (lowerLeft.second + height >= restraint) return;
	prevCells = cells;
	for (auto &i:coords) {
		int row = i.first;
		int col = i.second;
		i.first = width - col;
		i.second = row;
	}
	rotateUpdate();
}

void Block::moveLeft() {
	if (lowerLeft.second <= 0) return;
	prevCells = cells;
	int bugBreaker = cells.size();
	for (int i = 0; i < bugBreaker; i++) cells[i].col -= 1;
	lowerLeft.second -= 1;
	notifyObservers(SubscriptionType::blockChange);
}

void Block::moveRight(int restraint) {
	if (lowerLeft.second + width + 1 >= restraint) return;
	prevCells = cells;
	int bugBreaker = cells.size();
	for (int i = 0; i < bugBreaker; i++) cells[i].col += 1;
	lowerLeft.second += 1;
	notifyObservers(SubscriptionType::blockChange);
}

vector<Cell> Block::getCells() const {
	return this->cells;
}

bool Block::moveDown(int restraint) {
	prevCells = cells;
	if (lowerLeft.first + 1 >= restraint) return false;
	int bugBreaker = cells.size();
	for (int i = 0; i < bugBreaker; i++) cells[i].row += 1;
	lowerLeft.first += 1;
	notifyObservers(SubscriptionType::blockChange);
	return true;
}

Info Block::getInfo() const {
	return Info{prevCells, cells, -1, colour}; // Array of cells so display knows what to update
}

void Block::setLevel(int n){
	level = n;
}

string Block::getName(){
	return this->name;
}

Block::~Block() {}

ostream &operator<<(std::ostream &out, const Block&b) {
	bool printed = false;
	for (int i = 0; i <= b.height; i++) {
		for (int j = 0; j <= b.width; j++) {
			for (auto k:b.coords) {
				if (k.first == i && k.second == j) {
					out << b.dispChar;
					printed = true;
				}
			}
			if(!printed) out << ' ';
		}
		endl(out);
	}
	return out;
}


bool isInVec(vector<int> anycontainer, int testvalue){
	bool contains = find(anycontainer.begin(), anycontainer.end(), testvalue) != anycontainer.end();
	return !contains;
}

void Block::deleteCells(int theRow, int theCol){

	vector<int> index;
	vector<int> cols;

	cout << "wtf" << endl;
	int size = cells.size();

	cout << "size: " << size << endl;

	for(int i = 0; i < size; i++){
		cout << "indexsss: " << i << endl;

		if (cells[i].row == theRow && cells[i].col == theCol && isInVec(cols, cells[i].col)) {
				cout << "in if" << endl;
				index.emplace_back(i);
				cols.emplace_back(cells[i].col);
				cout << "fuvk ogf bu=ig b v you fucking asshole" << endl;
		}

	}

	cout << "wtff" << endl;
	int size2 = index.size();
	for (int j = 0; j < size2; j++) {
		this->cells.erase(cells.begin()+index[j]);
		for (int i = j+1; i < size2;  i++) {
				index[i] -= 1;
		}
	}

}


int Block::updateCells(int rows, int cols){
	cout << "YO FAMY" << endl;
	// prevCells = cells;
	for (int i = 0; i < cols; i++) {
		cout << "pls call" << endl;
		deleteCells(rows, i);
	}

	cout << "YO FAMY" << endl;
	notifyObservers(SubscriptionType::blockChange);
	if ( !(this->cells.empty()) ){
		return this->level;
	} else {
		return -1;
	}
}

int Block::getHeight() const {
	return height;
}

int Block::getWidth() const {
	return width;
}
