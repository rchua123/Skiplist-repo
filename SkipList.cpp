/*
 * Name: Ryan Chua
 * Class: SkipList.cpp
 * This SkipList class works as a two dimensional doubly linked list. There are rows which 
 * are labeled as levels: (Level: n) and elements in the skiplist are linked from side to side
 * as well as up and down. One feature of this class is adding elements to the first level. Then
 * there is a 50% chance that the element is also added one level above. It will keep going one level
 * above until it reaches its max depth or the 50% chance is false. There is also a remove function
 * that will remove one element from the whole list. There is also a contains method which indicates
 * whether the element is in the skiplist or not.
 */

#include <iostream>
#include <cstdlib>
#include <climits>
#include <cassert>

#include "SkipList.h"

using namespace std;

/**
* SkipList constructor
* Pre-Conditions: None
* 
* @param depth is the number of levels within the list
*/
SkipList::SkipList(int depth) {

	this->depth = depth;
	frontGuards = new SNode *[depth]; //depth level of heads
	rearGuards = new SNode *[depth]; // depth level of tails
	
	//build a list of doubly linked lists. 
	for(int i = 0; i < depth; i++){
		frontGuards[i] = new SNode(INT_MIN);
		rearGuards[i] = new SNode(INT_MAX);

		//link frontGuards and rearGuards together
		frontGuards[i]->next = rearGuards[i];
		rearGuards[i]->prev = frontGuards[i];
		
		//Make connections with different levels. If only one level exist, then no connection needed.
		if(i >= 1){
			frontGuards[i]->downLevel = frontGuards[i-1];
			frontGuards[i-1]->upLevel = frontGuards[i];
			rearGuards[i]->downLevel = rearGuards[i-1];
			rearGuards[i-1]->upLevel = rearGuards[i];
		}
        
	}
	//Set the tails to nullptr to indicate the end.
	frontGuards[0]->downLevel = nullptr;
	frontGuards[depth-1]->upLevel = nullptr;
	rearGuards[0]->downLevel = nullptr;
	rearGuards[depth-1]->upLevel = nullptr;
}

/**
* SkipList Deconstructor
*/
SkipList::~SkipList() {

	for(int i = 0; i < depth; i++){
		SNode *current = frontGuards[i]; //start at head

		while(current != nullptr){
			SNode *tempNext = current->next;
			delete current;
			current = tempNext;
		}
	}

	delete [] frontGuards;
	delete [] rearGuards;
}

/**
* SNode constructor
* Pre-Conditions: None
* Initiates data and makes next, prev, upLevel, downLevel to nullptr.
*
* @param data the node's stored int data.
*/
SkipList::SNode::SNode(int data) {
	this->data = data;
	this->next = nullptr;
    this->prev = nullptr;
    this->upLevel = nullptr;
    this->downLevel = nullptr;
}

/**
* Add method
* Pre-Conditions: Data will be added as long as if there are no duplicates
* This method add elements to the first level of the list. It uses the helper method
* addBefore to do the actually adding to list. Then it will determine if the element
* should be added to a level above. It will keep going until alsoHigher() becomes false.
*
* @param data integer that is added to the list.
* @return true if it was add and false if otherwise.
*/
bool SkipList::Add(int data){

	SNode *current = frontGuards[0];
	SNode *nextNode = current->next;

	//tranverse through list until given data is greater than the next data.
	while(nextNode->next != nullptr){
		//return false if there is a duplicate
		if(nextNode->data == data)
			return false;
		else if(nextNode->data < data)
			nextNode = nextNode->next;
		else
			break;
	}		

	SNode *newNode = new SNode(data);
	//insert the new node handling the data into the list.
	addBefore(newNode, nextNode);
	
	//This loop is to determine if the data should be inserted at a higher level.
	for(int i = 0; i < depth-1; i++){
		if(alsoHigher()){
			//When alsoHigher is true, duplicate data and add to upper level.
			SNode *addUpperNode = new SNode(data);
			//Doubly link addUpperNode to newNode.
			addUpperNode->downLevel = newNode;
			newNode->upLevel = addUpperNode;
			
			//go to the level above to the left most node
			SNode *nodeAbove = frontGuards[i+1]->next;
			//tranverse until given data is larger the next data.
			while(nodeAbove->next != nullptr && nodeAbove->data < data)
				nodeAbove = nodeAbove->next;
			//insert the addUpperNode to the upper level list.
			addBefore(addUpperNode, nodeAbove);
			//new node will be the node right above it.
			newNode = addUpperNode;
		}
		else
			break;
	}
	
	return true;
}

/**
* addBefore private method
* This is a helper method of the public Add method that does the process of adding
* a new node to the list.
*
* @param newNode that is going to be added to the list.
* @param nextNode the node to the right of newNode.
*/
void SkipList::addBefore(SNode *newNode, SNode *nextNode){
	assert(newNode != nullptr && nextNode != nullptr && newNode->data < nextNode->data);
	//have the new node link to the prev node (left). i.e. if {4,6} is a list and 5 is a new node,
	// link 5 to 4.
	newNode->prev = nextNode->prev;
	//then link the new node to the next node (right). i.e. link 5 to 6.
	newNode->next = nextNode;
	//Since it is doubly linked list, do the vice versa. i.e link 4 to 5.
	nextNode->prev->next = newNode;
	//same logic as last line. i.e. link 6 to 5.
	nextNode->prev = newNode;

	//after nodes have been linked
	assert(newNode->next == nextNode && nextNode->prev == newNode);
	assert(newNode->prev != nullptr && newNode->prev->data < newNode->data);
}

/**
* Remove method
* Pre-Conditions: data has to be in the skiplist.
* This method removes the inputted data from the skiplist starting from the highest level
* it is at.
* @param data desired element to check if it is in the skiplist.
* @return true if the data is removed and false if otherwise.
*/
bool SkipList::Remove(int data){
	
	for(int i = depth-1; i >=0; i--){
		//start at top
		SNode *current = frontGuards[i];
		//keep traversing until data is found
		while(current != nullptr){
			if(current->data == data){
				//while it is at the location of data all the downlevel pointers will have the same data
				//which is the one to be delete, so tranverse down while deleting each one from below levels. 
				while(current != nullptr){
					SNode *temp = current;
				
					current = current->downLevel;
					//link node to the left of temp to the right node of temp
					temp->prev->next = temp->next;
					
					delete temp;
				}
				return true;
			}
			else
				current = current->next;
		}
	}
	return false;
}


/**
* Contains method
* Pre-Conditions: none.
* This method indicates to the user if the data inputted is in the skiplist.
*
* @param data desired element to check if it is in the skiplist.
* @return true if the data is in there and false if otherwise.
*/
bool SkipList::Contains(int data){
	//start search at the top
	for(int i = depth - 1; i >= 0; i--){
		SNode *current = frontGuards[i];
		//traverse every ith row until it is found
		while(current != nullptr){
			if(current->data == data)
				return true;

			current = current->next;
		}
	}
	
    return false;
}

/**
* alsoHigher() method 
* This is a helper method for Add function to return true 50% of the time.
* Each node has a 50% chance of being at higher level.
* To go in depth, the (rand() % 2) + 1 gives either 1 or 2 randomly. 
*
* @return true if the random number is 2 and false if otherwise.
*/
bool SkipList::alsoHigher() const{
	return ((rand() % 2) + 1 == 2);
}


/**
* Overloaded '<<' operator method
* This method prints out each level in the form - Level: 0 -- INT_MIN, x_0,x_1,...x_n, INT_MAX,
* Depending on what input depth is will determine the number of levels that are outputted.
* @param out overloads cout.
* @param list SkipList object that allows the variables/methods in the class to be called.
* @return each level depending on depth with the list of numbers that are added.
*/
ostream &operator<<(ostream &out, const SkipList &list) {
	//print the highest level first and descend.
	for(int i = list.depth-1; i >= 0; i--){
		out << "Level: " << i << " -- ";
		
		SkipList::SNode *current = list.frontGuards[i];
		
		while(current != nullptr){
			out << current->data << ", ";
			current = current->next;
		}
		out << endl;
	}
	return out;
}

/**
* testPrivateMethods() private method
* Any private methods or varaibles that need to be tested to see if it works
* will be tested here.
*/
void SkipList::testPrivateMethods(){
	//check depth
	cout << "depth: " << depth << endl;
	//Test alsoHigher()
	for(int i = 0; i < 10; i++){
    	if(alsoHigher()){
            cout << "add element to upper level" << endl;
        }
        else
            cout << "dont add" << endl;
    }
}
