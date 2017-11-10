#include <iostream>
#include <string>
#include <vector>
#include <info.h>
#include <ControlVolume.h>
#include <AnalyticalSolution.h>
#include <algorithm>
#include <petscksp.h>
#include <cmath>
using namespace std;



int main()
{
	
	PetscInitialize(NULL,NULL,NULL,NULL);
	PlainWallInfo infoCenter;

	/*------------------------
		Malha do tipo CENTRAL 
	  -------------------------*/
	infoCenter.numberOfNodes=4;
	infoCenter.wallLength = 0.1;
	infoCenter.gridType = CENTER;
	infoCenter.thermalConduction = 50;
	infoCenter.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	infoCenter.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	infoCenter.beginBoundaryConditionInfo.push_back(100);
	infoCenter.endBoundaryConditionInfo.push_back(20);
	infoCenter.interfaceOperation = EQUIVALENT_RESISTANCE;

	ControlVolume centerMeshControlVolumeConrtol(infoCenter);
	AnalyticalSolution centerMeshAnalyticalSolution(infoCenter);
	centerMeshAnalyticalSolution.writeSolutionToCsv("../results/first_exercise", "centerMesh4NodesAN");
	centerMeshControlVolumeConrtol.writeSolutionToCsv("../results/first_exercise", "centerMesh4NodesVC");
	
	ofstream pFile;
	pFile.open("../results/first_exercise/table_error_center.csv",fstream::app);
	pFile << "numberOfNodes,maximum_error" << endl;
	pFile << scientific;
	for (int i = 2; i <= 10; ++i)
	{
		infoCenter.numberOfNodes=i;
		ControlVolume auxiliarControlVolume(infoCenter);
		AnalyticalSolution auxiliarAnalyticalSolution(infoCenter);
		vector<double> errorsVec;
		for (int j = 0; j < i; j++)
		{
			errorsVec.push_back(abs(auxiliarAnalyticalSolution[j] - auxiliarControlVolume.getTemperature(j)));
		}
		double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
		pFile << i << ", ";
		pFile << setprecision(16) << maximumError << endl;
	}
	pFile.close();


	PlainWallInfo infoBoth;

	/*------------------------
		Malha do tipo BOTH 
	  -------------------------*/
	infoBoth.numberOfNodes = 4;
	infoBoth.wallLength = 0.1;
	infoBoth.gridType = BOTH;
	infoBoth.thermalConduction = 50;
	infoBoth.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	infoBoth.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	infoBoth.beginBoundaryConditionInfo.push_back(100);
	infoBoth.endBoundaryConditionInfo.push_back(20);
	infoBoth.interfaceOperation = EQUIVALENT_RESISTANCE;

	ControlVolume bothMeshControlVolume(infoBoth);
	AnalyticalSolution bothMeshAnalyticalSolution(infoBoth);
	bothMeshAnalyticalSolution.writeSolutionToCsv("../results/first_exercise", "bothMesh4NodesAN");
	bothMeshControlVolume.writeSolutionToCsv("../results/first_exercise", "bothMesh4NodesVC");
	
	
	// pFile.open("../results/first_exercise/table_error_both.csv",fstream::app);
	// pFile << "numberOfNodes,maximum_error" << endl;
	// pFile << scientific;
	// for (int i = 2; i <= 10; ++i)
	// {
	// 	infoBoth.numberOfNodes=i;
	// 	ControlVolume auxiliarControlVolume(infoBoth);
	// 	AnalyticalSolution auxiliarAnalyticalSolution(infoBoth);
	// 	vector<double> errorsVec;
	// 	for (int j = 0; j < i; j++)
	// 	{
	// 		errorsVec.push_back(abs(auxiliarAnalyticalSolution[j] - auxiliarControlVolume.getTemperature(j)));
	// 	}
	// 	double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
	// 	pFile << i << ", ";
	// 	pFile << setprecision(16) << maximumError << endl;
	// }
	// pFile.close();

	PetscFinalize();
	return 0;
}