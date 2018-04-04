#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <info.h>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

class Mesh
{
public:
	Mesh(int numberOfNodes, double wallLength, GridType gridType);
	
	//get functions
	double centerPoint(int controlVolumeIndex);
	double eastPoint(int controlVolumeIndex);
	double westPoint(int controlVolumeIndex);
	double westFrontier(int controlVolumeIndex);
	double eastFrontier(int controlVolumeIndex);
	double eastDistance(int controlVolumeIndex);
	double eastPlusDistance(int controlVolumeIndex);
	double eastMinusDistance(int controlVolumeIndex);
	double westDistance(int controlVolumeIndex);
	double westPlusDistance(int controlVolumeIndex);
	double westMinusDistance(int controlVolumeIndex);
	
	int getNumberOfNodes();
	double getWallLength();
	FrontierType getBeginFrontierType();
	FrontierType getEndFrontierType();
	void addPlainWall(int numberOfNodes, double wallLength, GridType gridType);




private:
	vector<double> positionOfNodes;
	vector<double> positionSurfaceOfElements;
	
	int numberOfNodes;
	double wallLength;
	
	void centerTypeGridGenerator();
	void westTypeGridGenerator();
	void eastTypeGridGenerator();
	void bothTypeGridGenerator();
	void centerTypeGridAdder(int numberOfNodes, double wallLength);
	void eastTypeGridAdder(int numberOfNodes, double wallLength);
	void setPositionValue(int controlVolumeIndex, double value);
	void setSurfaceValue(int surfaceIndex, double value);
	
};

#endif
