#ifndef THERMAL_C_H
#define THERMAL_C_H

#include <vector>
#include <Mesh.h>
using namespace std;


class ThermalConduction
{
public:
	//Construtor
	ThermalConduction(int numberOfNodes, double k);
	ThermalConduction(int numberOfNodes1,double k1, int numberOfNodes2, double k2);
	ThermalConduction(vector<double> inputThermalConductions);
		
	
	//get functions
	double operator[](const int ControlVolumeIndex);
	double getWestInterface(Mesh mesh, int ControlVolumeIndex, InterfaceOperation operation);
	double getEastInterface(Mesh mesh, int index, InterfaceOperation operation);
	
private:
	vector<double> nodeThermalConductions;
	

	//set functions
	void setNodeThermalConduction(int ControlVolumeIndex, double k);
};

#endif