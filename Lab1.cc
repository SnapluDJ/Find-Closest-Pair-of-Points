//
// LAB1.CC
// Main driver code for CSE 241 Lab #1.
//
// WARNING: this file will be replaced with the unmodified
// original by the auto-grader and at turn-in time!

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>

#include <cstdlib>
#include <cmath>
#include <ctime>

#include "ClosestPairDC.h"
#include "ClosestPairNaive.h"

#include "Point.h"
#include "Timer.h"
#include "Prng.h"

using namespace std;

// Random seed, used when generating points at random.
static int seed = 9371;

///////////////////////////////////////////////////////////////////////
// Comparator functions for sort()

// compares p1 and p2 based on the x-coord
struct _lessThanX {
  bool operator() (const Point *p1, const Point *p2)
  { 
    return (p1->isLeftOf(p2));
  }
} lessThanX;

// compares p1 and p2 based on the y-coord
struct _lessThanY {
  bool operator()(const Point *p1, const Point *p2)
  { 
    return (p1->isBelow(p2));
  }
} lessThanY;


//
// genPointsAtRandom()
// Generate nPoints points uniformly at random.
//
static void genPointsAtRandom(Point *points[], int nPoints,
			      PRNG *prng)
{
  double x = 0.0;
  double y = 0.0;

  double step = sqrt(nPoints);
  
  for (int j = 0; j < nPoints; j++)
    {
      // bump next point's X coordinate
      x += 10000.0 * (prng->nextDouble() - 0.5);
      
      // move the Y coordinate a random amount up,
      // while keeping it within limits [0 .. nPoints)
      y = (y + step * prng->nextDouble());
      while (y > nPoints)
	y -= nPoints;
	   
      Point *p = new Point;
      p->setX((int) rint(x)); // set x and y coords of point
      p->setY((int) rint(y));
      points[j] = p;
    }
}


//
// readXYPoints()
// Read a collection of points from an input file.
// The first line of the file should give the number of points
// to read.  Each subsequent line should give the coordinates of
// one point in the form "x y".
//
// RETURNS: array of pointers to Point structures
// SETS: *inPoints to length of array
//
static Point **readXYPoints(const char *pointFileName, int *inPoints)
{
  ifstream pointFile(pointFileName);
  int nPoints;
  
  if (!pointFile)
    {
      cerr << "Error: could not open point file '"
	   << pointFileName << "'" << endl;
      exit(1);
    }
  
  pointFile >> nPoints;
  
  if (nPoints < 2)
    {
      cerr << "Error: need at least two points!" << endl;
      exit(1);
    }
  
  Point **points = new Point * [nPoints];
  
  for (int j = 0; j < nPoints; j++)
    {
      int x, y;
      
      pointFile >> x;
      pointFile >> y;
      
      if (!pointFile)
	{
	  cerr << "Error: could not read point #" << j << endl;
	  exit(1);
	}
      
      Point *p = new Point(x,y);
      
      points[j] = p;
    }
  
  *inPoints = nPoints;
  
  return points;
}


int main(int argc, char *argv[])
{
  Point **points;      // array of refs to points
  int nPoints;         // number of points
  Timer timer;  
  string fileName;
  
  double averageTime=0;
  double minimumTime=9999;
  double maximumTime=0;
  double totalTime=0;

  double timeForDC=0;
  double timeForNaive=0;

  int k=100;

  // for (int i=0; i<1000; i++){

  // only argument is the input spec
  if (argc > 1)
    {
      fileName = argv[1];
    }
  else
    {
      cerr << "Syntax: Lab1 <filename>\n";
      exit(1);
    }
  


  // A filename argument of the form '@x', where x is a non-negative
  // integer, allocates x random points.  Any other argument is
  // assumed to be a file from which points are read.

  if (fileName[0] != '@')
    {
      points = readXYPoints(argv[1], &nPoints);
    }
  else
    {
      nPoints = strtoul(fileName.substr(1).c_str(), NULL, 10);
      
      // Use these lines if you want a different random set of points for each
      // run.  This code sets the seed for the RNG from the system clock time.
      //
      {
        time_t tp;
        time(&tp);
        seed = tp;
       }

      PRNG prng(seed);  // seed the random number generator
      
      points = new Point * [nPoints];
      genPointsAtRandom(points, nPoints, &prng);
    }
  
  if (nPoints < 2)
    {
      cerr << "ERROR: input must contain at least two points\n";
      exit(1);
    }
  

 // for (int i=0; i<k; i++){


  timer.start();
  
  //////////////////////////////////////////////////////////////////////////
  // CLOSEST-PAIR ALGORITHM STARTS HERE
  
  // The algorithm expects two arrays containing the same points.
  Point **pointsByX = new Point * [nPoints];
  Point **pointsByY = new Point * [nPoints];
  
  for (int j = 0; j < nPoints; j++) { pointsByX[j] = points[j]; pointsByY[j] = points[j]; }
  
  // NB: you should *not* have to call sort() in your
  // own code!
  sort(pointsByX, pointsByX + nPoints, lessThanX); // sort by x-coord to get X
  sort(pointsByY, pointsByY + nPoints, lessThanY); // sort by y-coord to get Y
  
  findClosestPair(pointsByX, pointsByY, nPoints);
  
  // CLOSEST-PAIR ALGORITHM ENDS HERE
  //////////////////////////////////////////////////////////////////////////
  
  timer.stop();

  //timeForDC+=timer.elapsedTime();
  cout << "For n = " << nPoints << ", the time is ";
  cout << timer.elapsedTime() << " milliseconds\n\n";

/*
  totalTime += timer.elapsedTime();
  if (timer.elapsedTime() < minimumTime)
	  minimumTime = timer.elapsedTime();
  if (timer.elapsedTime() > maximumTime)
	  maximumTime = timer.elapsedTime();
*/




  timer.start();
  
  ///////////////////////////////////////////////////////////////////////
  // NAIVE CLOSEST-PAIR ALGORITHM STARTS HERE
  
  findClosestPairNaive(points, nPoints);
  
  // NAIVE CLOSEST-PAIR ALGORITHM ENDS HERE
  ///////////////////////////////////////////////////////////////////////
  
  timer.stop();
  
  //timeForNaive+=timer.elapsedTime();
  cout << "For n = " << nPoints << ", the naive time is ";
  cout << timer.elapsedTime() << " milliseconds\n\n";
  



  delete [] pointsByX;   // free storage from array ptsByX of pt refs 
  delete [] pointsByY;   // free storage from array ptsByY of pt refs

  for (int j = 0; j < nPoints; j++)
    delete points[j];  // free storage from points themselves
  
  delete [] points;

/*
 averageTime= totalTime/100;
 cout<<"Average Time:"<<averageTime<<endl
		 <<"Minimum Time:"<<minimumTime<<endl
		 	 <<"Maximum Time"<<maximumTime<<endl;
*/
/*
   timeForDC/=1000*k;
   timeForNaive/=1000*k;
   cout<<"Time for DC:"<<timeForDC<<endl
		  <<"Time for Naive:"<<timeForNaive<<endl;
*/
  return 0;
}
