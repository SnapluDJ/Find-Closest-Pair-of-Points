//
// CLOSEST-PAIR-DC.CC 
// Implementation of the divide-and-conquer
// closest-pair algorithm

#include <iostream>
#include <limits>

#include "ClosestPairDC.h"
#include "ClosestPairNaive.h"

using namespace std;

// bigger than any possible interpoint distance
const double INFTY = numeric_limits<double>::max(); 
const int INITIAL = numeric_limits<int> :: max();

Point* p1=new Point(INITIAL,INITIAL); //store the final closest pair of points
Point* p2=new Point(0,0); //store the final closest pair of points

//
// findClosestPair()
//
// Given a collection of nPoints points, find and ***print***
//  * the closest pair of points
//  * the distance between them
// in the form "(x1, y1) (x2, y2) distance"
//

double combine(Point *ptsByY[], Point* midPoint, int n,
			 	 double lrDist)
{
	double minDist=lrDist;
	double d;
	Point** yStrip= new Point *[n];
	int index=0; //total elements in yStrip

	// Construct yStrip
	for (int i=0; i<n; i++)
		if (abs(ptsByY[i]->x() - midPoint->x()) < lrDist){
			yStrip[index]=ptsByY[i];
			++index;
		}

	//Search
	for (int j=0; j<=index-2; j++){
		int k=j+1;
		while ((k<=index-1) && ((yStrip[k]->y() - yStrip[j]->y()) < lrDist)){
			d=yStrip[k]->distance(yStrip[j]);
			if (d<minDist){
				minDist=d;
				if (d < p1->distance(p2)){ // update p1 and p2
					p1=yStrip[k];
					p2=yStrip[j];
				}
			}
			k++;
		}
	}

	return minDist;
}

double closestPair(Point *ptsByX[], Point *ptsByY[], int n)
{
	// when n=1, mid=0, if XL is declared at the very first beginning,
	// then XL= new Point *[-1], this will cause problem
	if (n==1) return INFTY;
	if (n==2){
		if (ptsByX[0]->distance(ptsByX[1]) < p1->distance(p2)){ //update p1 and p2
			p1=ptsByX[0];
			p2=ptsByX[1];
		}
		return ptsByX[0]->distance(ptsByX[1]);
	}

	int mid=n/2;
	double distL=INFTY, distR=INFTY, d=INFTY;

	// need to enlarge these arrays, because the last element of YL and the first
	// element of YR are overlapping if declare like this "[mid-1] and [n-mid-1]"
	Point** XL= new Point *[mid];
	Point** XR= new Point *[n-mid];
	Point** YL= new Point *[mid];
	Point** YR= new Point *[n-mid];
	int index1=0, index2=0; //use to copy ptsByY into YL and YR

	// Copy ptsByX into XL and XR
	for (int i=0; i<mid; i++)
		XL[i]=ptsByX[i];
	for (int j=0; j<n-mid; j++)
		XR[j]=ptsByX[j+mid];

	// Copy ptsByY into YL and YR
	for (int i=0; i<n; i++)
		if (ptsByY[i]->isLeftOf(ptsByX[mid])){
			YL[index1]=ptsByY[i];
			++index1;
		}
		else{
			YR[index2]=ptsByY[i];
			++index2;
		}

	//Divide
	distL=closestPair(XL, YL, mid);
	distR=closestPair(XR, YR, n-mid);

	d=distL<distR?distL:distR;

	return combine(ptsByY, ptsByX[mid], n, d);
}

void findClosestPair(Point *pointsByX[], Point *pointsByY[],
		     int nPoints)
{

	double closestDist=INFTY;
	int flag1=0,flag2=0; //use for print

	closestDist=closestPair(pointsByX, pointsByY, nPoints);

	// Print
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
