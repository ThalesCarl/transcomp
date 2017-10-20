#include <iostream>
#include <string>
#include <vector>
#include <info.h>
#include <ControlVolume.h>
#include <AnalyticalSolution.h>
#include <petscksp.h>
using namespace std;



int main()
{
	
	PetscInitialize(NULL,NULL,NULL,NULL);
	PlainWallInfo firstExerciseInfo;

	firstExerciseInfo.numberOfNodes=10;
	firstExerciseInfo.wallLength = 0.1;
	firstExerciseInfo.gridType = CENTER;
	firstExerciseInfo.thermalConduction = 50;
	firstExerciseInfo.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	firstExerciseInfo.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	firstExerciseInfo.beginBoundaryConditionInfo.push_back(100);
	firstExerciseInfo.endBoundaryConditionInfo.push_back(20);
	firstExerciseInfo.interfaceOperation = EQUIVALENT_RESISTANCE;

	cout << "Mesh = CENTER, BC_begin = PRESCRIBED_TEMPERATURE(100), BC_end = PRESCRIBED_TEMPERATURE(20), n = 10 wl = 0.1" << endl;
	ControlVolume firstExerciseControlVolumeWithCenterMesh(firstExerciseInfo);
	AnalyticalSolution analyticalSolutionWithCenterMesh(firstExerciseInfo);	
	analyticalSolutionWithCenterMesh.printSolutionOnTheScreen();
	firstExerciseControlVolumeWithCenterMesh.printSolutionOnTheScreen();

	firstExerciseInfo.gridType = BOTH;
	cout << endl << "Mesh = BOTH, BC_begin = PRESCRIBED_TEMPERATURE(100), BC_end = PRESCRIBED_TEMPERATURE(20), n = 10 wl = 0.1" << endl;
	ControlVolume firstExerciseControlVolumeWithBothMesh(firstExerciseInfo);
	AnalyticalSolution analyticalSolutionWithBothMesh(firstExerciseInfo);
	analyticalSolutionWithBothMesh.printSolutionOnTheScreen();
	firstExerciseControlVolumeWithBothMesh.printSolutionOnTheScreen();

	PetscFinalize();
	return 0;
}