#include <ThermalConduction.h>

ThermalConduction::ThermalConduction(int numberOfNodes, double k)
{
	this -> nodeThermalConductions.resize(numberOfNodes);
	for(int i = 0; i < numberOfNodes; i++)
	{
		setNodeThermalConduction(i,k);
	}
}

ThermalConduction::ThermalConduction(vector<double> inputThermalConductions)
{
	this -> nodeThermalConductions.resize(inputThermalConductions.size());
	for(int i = 0; i < inputThermalConductions.size(); i++)
	{
		setNodeThermalConduction(i,inputThermalConductions[i]);
	}
}

ThermalConduction(int numberOfNodes1,double k1, int numberOfNodes2, double k2);
{
	this -> nodeThermalConductions.resize(numberOfNodes1+numberOfNodes2);
	for(int i = 0; i < numberOfNodes1, i++)
	{
		setNodeThermalConduction(i,k1);
	}
	for(int i = numberOfNodes1; i< numberOfNodes1 + numberOfNodes2; i++)
	{
		setNodeThermalConduction(i,k2);
	}
}

double ThermalConduction::operator[](const int index)
{
	return this -> nodeThermalConductions[index];
}

void ThermalConduction::setNodeThermalConduction(int index, double k)
{
	this -> nodeThermalConductions[index] = k;
}

double ThermalConduction::getWestInterface(Mesh mesh, int index, InterfaceOperation operation)
{
	double numerator, denominator;
	if (operation == EQUIVALENT_RESISTANCE)
	{
		numerator = mesh.westDistance(index);
		denominator =  (mesh.westMinusDistance(index))/(this -> nodeThermalConductions[index - 1]);
		denominator += (mesh.westPlusDistance(index)) /(this -> nodeThermalConductions[index]);
	} 
	else if(operation == LINEAR_INTERPOLATION)
	{
		numerator = this -> nodeThermalConductions[index - 1] * mesh.westMinusDistance(index);
		numerator += this -> nodeThermalConductions[index] * mesh.westPlusDistance(index);
		denominator =  mesh.westDistance(index);	
	}
	else 
	{
		cout << "Invalid Interface Operation" << endl;
		numerator = denominator = 0.0;
	}
	return numerator/denominator;

}

double ThermalConduction::getEastInterface(Mesh mesh, int index, InterfaceOperation operation)
{
	double numerator, denominator;
	if (operation == EQUIVALENT_RESISTANCE)
	{
		numerator = mesh.eastDistance(index);
		denominator =  (mesh.eastMinusDistance(index))/(this -> nodeThermalConductions[index]);
		denominator += (mesh.eastPlusDistance(index)) /(this -> nodeThermalConductions[index + 1]);
	} 
	else if(operation == LINEAR_INTERPOLATION)
	{
		numerator =  this -> nodeThermalConductions[index] * mesh.eastMinusDistance(index);
		numerator += this -> nodeThermalConductions[index + 1] * mesh.eastPlusDistance(index);
		denominator = mesh.eastDistance(index);
	}
	else 
	{
		cout << "Invalid Interface Operation" << endl;
		numerator = denominator = 0.0;
	}
	return numerator/denominator;

}