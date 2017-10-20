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

#endif
