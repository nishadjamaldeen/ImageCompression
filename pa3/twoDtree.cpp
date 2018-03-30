
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include <math.h>
#include <iostream>

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}



twoDtree::twoDtree(PNG & imIn){ 


	height = imIn.height();
	width = imIn.width();


	stats s(imIn);
	pair <int, int> ul;
	pair <int, int> lr;

	ul = pair<int, int>(0, 0);
	lr = pair<int, int>(width - 1, height - 1);

	root = buildTree(s, ul, lr);

}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {

	// Create new node to be returned to caller function as appropriate child node
	Node * curr = new Node(ul, lr, s.getAvg(ul, lr));
	curr -> left = nullptr;
	curr -> right = nullptr;

	//If node is a single pixel, return said node 
	if (ul.first == lr.first && ul.second == lr.second) return curr;
	// Variable to determine which way to partition
	bool vertical = true;
	// Initial values for the partition
	int vert = ul.first;
	int horiz = ul.second;
	// Initialize minimum variance - considers if we are dealing with a nx1 or 1xm matrix
	long R1;
	long R2;

	if (ul.first == lr.first){
		R1 = s.getScore(ul, pair<int, int>(lr.first, horiz));
		R2 = s.getScore(pair<int, int>(ul.first, horiz + 1), lr);
		vertical = false;
	} else {
		R1 = s.getScore(ul, pair<int, int>(vert, lr.second));
		R2 = s.getScore(pair<int, int>(vert+1, ul.second), lr);
		vertical = true;
	}

	long minVar = R1 + R2;
	// Tests all partitions to determine partition with minimum variance
	for (int i = vert + 1; i < lr.first; i++){
		R1 = s.getScore(ul, pair<int, int>(i, lr.second));
		R2 = s.getScore(pair<int, int>(i+1, ul.second), lr);

		if (R1 + R2 <= minVar){
			minVar = R1 + R2;
			vertical = true;
			vert = i;
		}
	}	

	for (int j = horiz + 1; j < lr.second; j++){
		R1 = s.getScore(ul, pair<int, int>(lr.first, j));
		R2 = s.getScore(pair<int, int>(ul.first, j + 1), lr);

		if (R1 + R2 <= minVar){
			minVar = R1 + R2;
			vertical = false;
			horiz = j;
		}
	}

	// Tie breaking in the event the parent node is passed through
	// if ((vert == ul.first || vert == lr.first)){
	// 	vert = ul.first + (lr.first - ul.first)/2.0;
	// }
	
	// else if ((horiz == ul.second || horiz == lr.second)){
	// 	horiz = ul.second + (lr.second - ul.second)/2.0;
	// }
	// Setting up children for the current node recursively
	if (vertical){
		curr -> left = buildTree(s, ul, pair<int, int>(vert, lr.second));
		curr -> right = buildTree(s, pair<int, int>(vert + 1, ul.second), lr);
		 
	} else {
		curr -> left = buildTree(s, ul, pair<int, int>(lr.first, horiz));
		curr -> right = buildTree(s, pair<int, int>(ul.first, horiz + 1), lr);
	}

	return curr;

}


 
PNG twoDtree::render(){
	PNG image(width, height);
	render(root, image);
	return image;
}

void twoDtree::render(Node * curr, PNG & image){

	if (curr == nullptr) return;

	if (curr -> left == nullptr && curr -> right == nullptr){		
		for (int i = curr -> upLeft.first; i <= curr -> lowRight.first; i++){
			for (int j = curr -> upLeft.second; j <= curr -> lowRight.second; j++){
				*(image.getPixel(i,j)) = curr -> avg;
			}
		}
	} else {
		render(curr -> left, image);
		render(curr -> right, image);
	}
	// else if (curr -> left != nullptr && curr -> right != nullptr){
	// 	render(curr -> left, image);
	// 	render(curr -> right, image);
	// } else if (curr -> left != nullptr) {
	// 	render(curr -> left, image);
	// } else if (curr -> right != nullptr){
	// 	render(curr -> right, image);
	// }
}

void twoDtree::prune(double pct, int tol){
	pruneRecursive(root, pct, tol);
}

void twoDtree::pruneRecursive(Node * curr, double pct, int tol){
	if (curr == nullptr) return;
	double numNodes = 0;
	double accepted = pruneHelper(curr, tol, curr -> avg, numNodes);
	if (accepted/(numNodes) >= pct){
		clear(curr);
		return;
	} else {
		pruneRecursive(curr -> left, pct, tol);
		pruneRecursive(curr -> right, pct, tol);
	}
}

double twoDtree::pruneHelper(Node * curr, int tol, RGBAPixel & avg, double & numNodes){

	if (curr == nullptr) return 0.0;
	
	if (curr -> left == nullptr && curr -> right == nullptr){
		numNodes += 1;
		double distance = pow(curr -> avg.r - avg.r, 2) + pow(curr -> avg.b - avg.b, 2) + pow(curr -> avg.g - avg.g, 2);
		if (distance <= tol) return 1.0;
		else return 0.0;
	} else {
		double leftNum = pruneHelper(curr -> left, tol, avg, numNodes);
		double rightNum = pruneHelper(curr -> right, tol, avg, numNodes);
		return leftNum + rightNum;
	}
}


void twoDtree::clear() {
	clear(root);
}

void twoDtree::clear(Node * curr){
	if (curr == nullptr || (curr -> left == nullptr && curr -> right == nullptr)) return;
	else if (curr -> left == nullptr){
		clear(curr -> right);
		delete curr -> right;
		curr -> right = nullptr;
	} else if (curr -> right == nullptr){
		clear(curr -> left);
		delete curr -> left;
		curr -> left = nullptr;
	} else {
		clear (curr -> left);
		delete curr -> left;
		curr -> left = nullptr;
		clear (curr -> right);
		delete curr -> right;
		curr -> right = nullptr;
	}

	// if (curr == nullptr) return;
	// clear(curr -> left);
	// clear(curr -> right);
	// delete curr;
	// curr = nullptr;
	
}


void twoDtree::copy(const twoDtree & orig){
	width = orig.width;
	height = orig.height;
	root = copy(orig.root);

}


twoDtree::Node * twoDtree::copy(Node * otherCurr){
	if (otherCurr == nullptr) return nullptr;
	else{
		Node * thisNode = new Node(otherCurr -> upLeft, otherCurr -> lowRight, otherCurr -> avg);
		thisNode -> left = copy(otherCurr -> left);
		thisNode -> right = copy(otherCurr -> right);
		return thisNode;
	}
}





