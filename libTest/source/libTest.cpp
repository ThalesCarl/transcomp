#define BOOST_TEST_MODULE TestModule

#include "Test.h"
#include "Mesh.h"
// #include "ThermalConduction.h"
// #include "Boundary.h"
// #include "LinearSystem.h"
// #include "AnalyticalSolution.h"
// #include "ControlVolume.h"

TestCase(MeshAdderTest)
{
	Mesh mesh(3,1,WEST);
	cout << "Generation: " << endl;
	cout << "Positions: ";
	for (int i = 0; i < mesh.getNumberOfNodes(); ++i)
	{
		cout << mesh.centerPoint(i) << " ";
	}
	cout << endl << "Surfaces: ";
	cout << mesh.westFrontier(0) << " ";
	for (int i = 0; i < mesh.getNumberOfNodes(); ++i)
	{
		cout << mesh.eastFrontier(i) << " ";
	}
	cout << endl << "Addition:" << endl;
	mesh.addPlainWall(2,1,CENTER);


}