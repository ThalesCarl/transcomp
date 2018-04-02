#ifndef INFO_H
#define INFO_H

#include <vector>

enum BoundaryCondition {PRESCRIBED_TEMPERATURE,PRESCRIBED_FLUX,CONVECTION};
enum GridType {CENTER, WEST, EAST, BOTH};
enum InterfaceOperation {EQUIVALENT_RESISTANCE, LINEAR_INTERPOLATION};
enum FrontierType {CONNECTED, UNCONNECTED, UNEXPECTED};

struct PlainWallInfo
{
	int numberOfNodes;
	double wallLength;
	GridType gridType;
	double thermalConduction;
	BoundaryCondition beginBoundaryConditionType;
	BoundaryCondition endBoundaryConditionType;
	std::vector<double> beginBoundaryConditionInfo;
	std::vector<double> endBoundaryConditionInfo;
	InterfaceOperation interfaceOperation;
};

struct DoublePlainWallInfo
{
	int numberOfNodes1;
	int numberOfNodes2;
	double wallLength1;
	double wallLength2;
	GridType gridType1;
	GridType gridType2;
	double thermalConduction1;
	double thermalConduction2;
	BoundaryCondition beginBoundaryConditionType;
	BoundaryCondition endBoundaryConditionType;
	std::vector<double> beginBoundaryConditionInfo;
	std::vector<double> endBoundaryConditionInfo;
	InterfaceOperation interfaceOperation;
};

struct TransientPlainWallInfo
{
	int numberOfNodes;
	double wallLength;
	GridType gridType;
	double thermalConduction;
	double density;
	double cp;
	double biotNumber;
	double timeStep;
	double initialTemperature;
	BoundaryCondition beginBoundaryConditionType;
	BoundaryCondition endBoundaryConditionType;
	std::vector<double> beginBoundaryConditionInfo;
	std::vector<double> endBoundaryConditionInfo;
	InterfaceOperation interfaceOperation;
};

struct PlainWallNonLinearInfo
{
	int numberOfNodes;
	vector <double> thermalConductionCoefficients;
	double wallLength;
	GridType gridType;
	double thermalConduction;
	BoundaryCondition beginBoundaryConditionType;
	BoundaryCondition endBoundaryConditionType;
	std::vector<double> beginBoundaryConditionInfo;
	std::vector<double> endBoundaryConditionInfo;
	InterfaceOperation interfaceOperation;
};



#endif
