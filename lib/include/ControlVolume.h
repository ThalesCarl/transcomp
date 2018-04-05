#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <info.h>
#include <ThermalConduction.h>
#include <Mesh.h>
#include <Boundary.h>
#include <LinearSystem.h>
#include <ConvergenceCriteria.h>
#include <algorithm>
using namespace std;
using vector_double_2d = vector<vector<double>>;

class ControlVolume
{
public:

	ControlVolume(PlainWallInfo data);
	ControlVolume(DoublePlainWallInfo data);
	ControlVolume(PlainWallNonLinearInfo data);

	//get functions
	void writeSolutionToCsv(string directory, string fileName);
	void printSolutionOnTheScreen();
	double getPosition(int controlVolumeIndex);
	double getTemperature(int controlVolumeIndex);

private:		
	Mesh mesh;
	ThermalConduction vectorK;
	Boundary boundaries;
	LinearSystem solver;
	vector<double> temperatureField;
	InterfaceOperation interfaceOperation;
	vector<double> oldTemperatureField;	

	// ConvergenceCriteria selectConvergenceCriteria(ConvergenceCriteriaType);
	void beginProcessor();
	void endProcessor();
};