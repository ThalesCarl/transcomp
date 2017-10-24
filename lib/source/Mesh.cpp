#include "Mesh.h"

Mesh::Mesh(int numberOfNod, double wallLen, GridType gridT):
	numberOfNodes((numberOfNod >= 1)? numberOfNod: -30), 
	wallLength(wallLen)
{
	this -> positionOfNodes.resize(numberOfNodes);
	this -> positionSurfaceOfElements.resize(numberOfNodes + 1);

	switch(gridT)
	{
		case CENTER:
			centerTypeGridGenerator();
		break;
		case EAST:
			eastTypeGridGenerator();
		break;
		case WEST:
			westTypeGridGenerator();
		break;
		case BOTH:
			bothTypeGridGenerator();
		break;
		default:
			cout << "Grid Type Error while creating the mesh" << endl;
	}	
}

void Mesh::centerTypeGridGenerator()
{
	int numberOfNodes = this -> numberOfNodes;
	double wallLength = this -> wallLength;
	double delta;	
	delta = wallLength/numberOfNodes;

	double sum = 0.5 * delta;
	for (int i = 0; i < numberOfNodes; i++)
	{
		setPositionValue(i,sum);
		sum += delta;
	}
	sum = 0;
	for (int i = 0; i <= numberOfNodes; i++)
	{
		setSurfaceValue(i,sum);
		sum += delta;
	}
}

void Mesh::westTypeGridGenerator()
{	
	int numberOfNodes = this -> numberOfNodes;
	double wallLength = this -> wallLength;
	double delta = wallLength/(numberOfNodes - 0.5);
	double sum = 0;
	for (int i = 0; i < numberOfNodes; i++)
	{
		setPositionValue(i,sum); 
		sum += delta;
	}
	sum = 0;
	setSurfaceValue(0,sum);
	sum = 0.5*delta;
	for (int i = 1; i <= numberOfNodes; i++)
	{
		setSurfaceValue(i,sum);
		sum += delta;
	}
}

void Mesh::eastTypeGridGenerator()
{
	int numberOfNodes = this -> numberOfNodes;
	double wallLength = this -> wallLength;
	double delta = wallLength/(numberOfNodes - 0.5);
	double sum = 0.5*delta;
	for (int i = 0; i < numberOfNodes; i++)
	{
		setPositionValue(i,sum);
		sum += delta;
	}
	sum = 0;
	for (int i = 0; i < numberOfNodes; i++)
	{
		setSurfaceValue(i,sum);
		sum += delta;
	}
	sum -= 0.5*delta;
	setSurfaceValue(numberOfNodes,sum);
}

void Mesh::bothTypeGridGenerator()
{
	int numberOfNodes = this -> numberOfNodes;
	double wallLength = this -> wallLength;
	double delta = wallLength/(numberOfNodes - 1);
	double sum = 0;
	for (int i = 0; i < numberOfNodes; i++)
	{
		setPositionValue(i,sum);
		sum += delta;
	}
	sum = 0;
	setSurfaceValue(0,sum);
	sum = 0.5*delta;
	for (int i = 1; i < numberOfNodes; i++)
	{
		setSurfaceValue(i,sum);
		sum += delta;
	}
	sum -= 0.5*delta;
	setSurfaceValue(numberOfNodes, sum);
}

void Mesh::setPositionValue(int index, double value)
{
	this -> positionOfNodes[index] = value;
}

void Mesh::setSurfaceValue(int index, double value)
{
	this -> positionSurfaceOfElements[index] = value;
}

double Mesh::centerPoint(int controlVolumeIndex)
{
	return this -> positionOfNodes[controlVolumeIndex];
}

double Mesh::eastPoint(int controlVolumeIndex)
{
	int maximum_controlVolumeIndex;
	maximum_controlVolumeIndex = this -> positionOfNodes.size();
	
	if (controlVolumeIndex < maximum_controlVolumeIndex)
		return this -> positionOfNodes[controlVolumeIndex + 1];
	else
	{
		cout << "invalid memory location" << endl;
		return -1;
	}
}

double Mesh::westPoint(int controlVolumeIndex)
{
	if (controlVolumeIndex > 0)
		return this -> positionOfNodes[controlVolumeIndex - 1];
	else
	{
		cout << "invalid memory location" << endl;
		return -1;
	}
}

double Mesh::westFrontier(int controlVolumeIndex)
{
	return this -> positionSurfaceOfElements[controlVolumeIndex];
}

double Mesh::eastFrontier(int controlVolumeIndex)
{
	return this -> positionSurfaceOfElements[controlVolumeIndex + 1];
}

double Mesh::eastDistance(int controlVolumeIndex)
{
	return eastPoint(controlVolumeIndex) - centerPoint(controlVolumeIndex);
}

double Mesh::eastPlusDistance(int controlVolumeIndex)
{
	return eastPoint(controlVolumeIndex) - eastFrontier(controlVolumeIndex);

}

double Mesh::eastMinusDistance(int controlVolumeIndex)
{
	return eastFrontier(controlVolumeIndex) - centerPoint(controlVolumeIndex);

}

double Mesh::westDistance(int controlVolumeIndex)
{
	return centerPoint(controlVolumeIndex) - westPoint(controlVolumeIndex);
}

double Mesh::westPlusDistance(int controlVolumeIndex)
{
	return centerPoint(controlVolumeIndex) - westFrontier(controlVolumeIndex);
}

double Mesh::westMinusDistance(int controlVolumeIndex)
{
	return westFrontier(controlVolumeIndex) - westPoint(controlVolumeIndex);
}

int Mesh::getNumberOfNodes()
{
	return this -> numberOfNodes;
}

double Mesh::getWallLength()
{
	return this -> wallLength;

}

FrontierType Mesh::getBeginFrontierType()
{
	FrontierType beginFrontierType;
	try
	{
		if(this -> positionOfNodes[0] == this -> positionSurfaceOfElements[0])
		{
			beginFrontierType = CONNECTED;
		} 
		else if (this -> positionOfNodes[0] != this -> positionSurfaceOfElements[0])
		{
			beginFrontierType = UNCONNECTED;
		}
		else
		{
			throw UNEXPECTED;
		}
	}
	catch (FrontierType e)
	{
		cout << "Begin Frontier Type = " << e << ". Setting to CONNECTED to prevent error" << endl;
		beginFrontierType = CONNECTED;
	}
	return beginFrontierType;
}

FrontierType Mesh::getEndFrontierType()
{
	int n = this -> numberOfNodes;
	FrontierType endFrontierType;
	try
	{
		if(this -> positionOfNodes[n] == this -> positionSurfaceOfElements[n + 1])
		{
			endFrontierType = CONNECTED;
		} 
		else if (this -> positionOfNodes[n] != this -> positionSurfaceOfElements[n + 1])
		{
			endFrontierType = UNCONNECTED;
		}
		else
		{
			throw UNEXPECTED;
		}
	}
	catch (FrontierType e)
	{
		cout << "Begin Frontier Type = " << e << ". Setting to CONNECTED to prevent error" << endl;
		endFrontierType = CONNECTED;
	}
	return endFrontierType;
}

void Mesh::addPlainWall(int numberOfNodes, double wallLength, GridType gridType)
{
	FrontierType tail = getEndFrontierType();
	if(tail == UNCONNECTED)
	{
		switch(gridType)
		{
			case WEST:
				cout << "Impossible to add this type of wall" << endl;
			break;
			case BOTH:
				cout << "Impossible to add this type of wall" << endl;
			break;
			case EAST:
				eastTypeGridAdder(numberOfNodes,wallLength);
			break;
			case CENTER:
				centerTypeGridAdder(numberOfNodes,wallLength);
			break;
			default:
				cout << "GridType error while trying to add wall" << endl;
		}
	}
	else
	{
		cout << "Impossible to add another Wall" << endl;
	}
}

void Mesh::centerTypeGridAdder(int numOfNod, double wallLeng)
{
	double delta = wallLeng/numOfNod;
	double beginReference = wallLength;

	int initialNode = this -> numberOfNodes;
	int finalNode = this -> numberOfNodes + numOfNod -1;
	this -> positionOfNodes.resize(finalNode + 1);
	this -> positionSurfaceOfElements.resize(finalNode + 2);
	
	double sum = beginReference + 0.5 * delta;
	for (int i = initialNode; i <= finalNode; i++)
	{
		setPositionValue(i,sum);
		sum += delta;
	}
	sum = beginReference + delta;
	for (int i = initialNode + 1; i <= finalNode + 1; i++)
	{
		setSurfaceValue(i,sum);
		sum += delta;
	}
	this -> numberOfNodes = finalNode + 1;
	this -> wallLength = sum - delta;

	
}
void Mesh::eastTypeGridAdder(int numOfNod, double wallLen)
{
	double delta = wallLen/(numOfNod - 0.5);
	double beginReference = wallLength;

	int initialNode = this -> numberOfNodes;
	int finalNode = this -> numberOfNodes + numOfNod -1;
	this -> positionOfNodes.resize(finalNode + 1);
	this -> positionSurfaceOfElements.resize(finalNode + 2);

	double sum = beginReference + 0.5*delta;
	for (int i = initialNode; i <= finalNode; i++)
	{
		setPositionValue(i,sum);
		sum += delta;
	}
	sum = beginReference + delta;;
	for (int i = initialNode + 1; i <= finalNode; i++)
	{
		setSurfaceValue(i,sum);
		sum += delta;
	}
	sum -= 0.5*delta;
	setSurfaceValue(finalNode + 1,sum);
	this -> numberOfNodes = finalNode + 1;
	this -> wallLength = sum;
}