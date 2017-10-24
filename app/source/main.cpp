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
	PlainWallInfo firstExerciseInfo;

	firstExerciseInfo.numberOfNodes=4;
	firstExerciseInfo.wallLength = 0.1;
	firstExerciseInfo.gridType = CENTER;
	firstExerciseInfo.thermalConduction = 50;
	firstExerciseInfo.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	firstExerciseInfo.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	firstExerciseInfo.beginBoundaryConditionInfo.push_back(100);
	firstExerciseInfo.endBoundaryConditionInfo.push_back(20);
	firstExerciseInfo.interfaceOperation = EQUIVALENT_RESISTANCE;

	ControlVolume firstExerciseVC(firstExerciseInfo);
	AnalyticalSolution firstExerciseAN(firstExerciseInfo);
	firstExerciseAN.writeSolutionToCsv("../results", "firstExerciseCenterMesh4NodesAN");
	firstExerciseVC.writeSolutionToCsv("../results", "firstExerciseCenterMesh4NodesVC");

	
	ofstream pFile;
	pFile.open("../results/first_exercise_table_error.csv",fstream::app);
	pFile << "numberOfNodes,maximum_error" << endl;
	pFile << scientific;
	for (int i = 2; i <= 10; ++i)
	{
		firstExerciseInfo.numberOfNodes=i;
		ControlVolume auxiliarControlVolume(firstExerciseInfo);
		AnalyticalSolution auxiliarAnalyticalSolution(firstExerciseInfo);
		vector<double> errorsVec;
		for (int j = 0; j < i; j++)
		{
			errorsVec.push_back(abs(auxiliarAnalyticalSolution[j] - auxiliarControlVolume.getTemperature(j)));
		}
		double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
		pFile << i << ", ";
		pFile << setprecision(16) << maximumError << endl;
	}


	PetscFinalize();
	return 0;
}