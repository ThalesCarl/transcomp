#include <Boundary.h>

Boundary::Boundary(BoundaryCondition tBegin, BoundaryCondition tEnd, vector<double> begin, vector<double> end)
{
	setBegin(tBegin,begin);
	setEnd(tEnd,end);

}

double Boundary::getBeginBoundaryCondition()
{
	return this -> boundaryConditions.first.second;
}

double Boundary::getEndBoundaryCondition()
{
	return this -> boundaryConditions.second.second;

}

double Boundary::getBeginConvectionCoeficient()
{
	return this -> boundaryConditions.first.first;
}

double Boundary::getEndConvectionCoeficient()
{
	return this -> boundaryConditions.second.first;

}

void Boundary::setBegin(BoundaryCondition type, vector<double> info)
{
	if (type == CONVECTION)
	{
		this -> boundaryConditions.first.first = info[0];
		this -> boundaryConditions.first.second = info[1];		
	}
	else
	{
		this -> boundaryConditions.first.first = 0;
		this -> boundaryConditions.first.second = info[0];
	}
	this -> typeBegin = type;
}
void Boundary::setEnd(BoundaryCondition type, vector<double> info)
{
	if (type == CONVECTION)
	{
		this -> boundaryConditions.second.first = info[0];
		this -> boundaryConditions.second.second = info[1];		
	}
	else
	{
		this -> boundaryConditions.second.first = 0;
		this -> boundaryConditions.second.second = info[0];
	}
	this -> typeEnd = type;
}

BoundaryCondition Boundary::getTypeBegin()
{
	return this -> typeBegin;
}

BoundaryCondition Boundary::getTypeEnd()
{
	return this -> typeEnd;
}

