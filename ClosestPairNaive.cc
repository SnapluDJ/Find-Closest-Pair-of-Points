//
// CLOSEST-PAIR-NAIVE.CC
// Naive implementation of the closest-pair algorithm

#include <iostream>
#include <limits>

#include "ClosestPairNaive.h"

using namespace std;

// bigger than any possible interpoint distance
const double INFTY = numeric_limits<double>::max(); 

//
// findClosestPair()
//
// Given a collection of nPoints points, find and ***print***
//  * the closest pair of points
//  * the distance between them
// in the form "(x1, y1) (x2, y2) distance"
//

// INPUTS:
//  - points (not necessarily sorted)
//  - # of input points
//
void findClosestPairNaive(Point *points[], int nPoints)
{
  //
  // Your code here!
  //
	double closestDist=INFTY;
	Point* p1=new Point; //store the closest pair of points
	Point* p2=new Point; //store the closest pair of points
	int flag1=0,flag2=0; //use for print

	// search
	for (int i=0; i<nPoints-1; i++)
		for (int j=i+1; j<nPoints; j++){
			if (points[i]->distance(points[j]) < closestDist){
				closestDist=points[i]->distance(points[j]);
				p1=points[i];
				p2=points[j];
			}
		}

	// print
	if (p1->x() < p2->x()) flag1=1;
	if (p1->x() == p2->x()) flag1=2;
	if (p1->y() < p2->y()) flag2=1;

	if ((flag1==1) || (flag1==2 && flag2==1))
		cout<<"("<<p1->x()<<", "<<p1->y()<<")"<<" "
			<<"("<<p2->x()<<", "<<p2->y()<<")"<<" "<<closestDist<<endl;
	if ((flag1==0) || (flag1==2 && flag2==0))
		cout<<"("<<p2->x()<<", "<<p2->y()<<")"<<" "
			<<"("<<p1->x()<<", "<<p1->y()<<")"<<" "<<closestDist<<endl;

}
