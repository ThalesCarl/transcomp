#ifndef BC_H
#define BC_H

#include <iostream>
#include <vector>
#include <utility>
#include <info.h>
using namespace std;

class Boundary
{
public:
	Boundary(BoundaryCondition typeBegin, BoundaryCondition typeEnd, vector<double> beginBoundaryCondition, vector<double> endBoundaryCondition);
	double getBeginBoundaryCondition(); //Return prescribed temperature or flux or infinite Temperature
	double getEndBoundaryCondition();
	double getBeginConvectionCoeficient(); 
	double getEndConvectionCoeficient();
	BoundaryCondition getTypeBegin();
	BoundaryCondition getTypeEnd();

private:
	BoundaryCondition typeBegin, typeEnd;
	pair< pair<double,double>,pair<double,double>> boundaryConditions;
	
	//set functions
	void setBegin(BoundaryCondition type, vector<double> info);
	void setEnd(BoundaryCondition type, vector<double> info);
};

#endif