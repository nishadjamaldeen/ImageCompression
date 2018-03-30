
// File:        main.cpp
// Author:      cheeren
// Date:        2018-02-25
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets


#include "twoDtree.h"
#include "cs221util/RGBAPixel.h"
#include "cs221util/PNG.h"
#include <iostream>

using namespace cs221util;
using namespace std;

int main()
{

	// read in image
	PNG origIm1;
	origIm1.readFromFile("images/stanley-totem-poles.png");
	PNG origIm2;
	origIm2.readFromFile("images/ubc-totem-poles.png");
	PNG origIm3;
	origIm3.readFromFile("images/remb.png");
	PNG origIm4;
	origIm4.readFromFile("images/rosa.png");



	// PNG test;
	// test.readFromFile("images/test.png");
	// stats s(test);

	// int redSum = s.getSum('r', make_pair(1,0), make_pair(2,0));
	// int redSum2 = s.getSum('r', make_pair(2, 0), make_pair(2,2));

	// cout << redSum << endl;
	// cout << redSum2 << endl;


	cout << "Main 32" << endl;
	// use it to build a twoDtree
	twoDtree t1(origIm1);
	twoDtree tCopy1(t1);
	twoDtree t2(origIm2);
	twoDtree tCopy2(t2);
	twoDtree t3(origIm3);
	twoDtree tCopy3(t3);
	twoDtree t4(origIm4);
	twoDtree tCopy4(t4);

	cout << "Reached Main 43" << endl;

	// prune the twoDtree
	//where at least 100*X% of pixels are within Y of mean
	tCopy1.prune(0.95, 10000); 
	tCopy2.prune(0.95, 10000); 
	tCopy3.prune(0.95, 3000); 
	tCopy4.prune(0.95, 3000); 

	cout << "Main 52" << endl;
\
	
	// render the twoDtree
	PNG ppic1 = tCopy1.render();
	PNG ppic2 = tCopy2.render();
	PNG ppic3 = tCopy3.render();
	PNG ppic4 = tCopy4.render();



	ppic1.writeToFile("images/prunedstanleytotem95.png");
	ppic2.writeToFile("images/prunedubctotem95.png");
	ppic3.writeToFile("images/prunedremb95.png");
	ppic4.writeToFile("images/prunedrosa95.png");

  return 0;
}

