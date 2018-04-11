#include <AnalyticalSolution.h>

AnalyticalSolution::AnalyticalSolution(PlainWallInfo data): 
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes,data.thermalConduction),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());
	double temporaryTemperatureValue;
	for (int node = 0; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalFirstProblemTemperatureLaw(mesh.centerPoint(node));
		addToTemperatureField(node,temporaryTemperatureValue);
	}
}

AnalyticalSolution::AnalyticalSolution(DoublePlainWallInfo data): 
	mesh(data.numberOfNodes1, data.wallLength1, data.gridType1),
	vectorK(data.numberOfNodes1,data.thermalConduction1,data.numberOfNodes2,data.thermalConduction2),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	double temporaryTemperatureValue;	
	this -> temperatureField.resize(mesh.getNumberOfNodes());
	int numberOfNodes1 = mesh.getNumberOfNodes();
	for (int node = 0; node < numberOfNodes1; node++)
	{
		temporaryTemperatureValue = evalSecondProblemTemperatureLaw(mesh.centerPoint(node),data);
		addToTemperatureField(node,temporaryTemperatureValue);
	}
	this -> mesh.addPlainWall(data.numberOfNodes2, data.wallLength2, data.gridType2);
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());

	for (int node = numberOfNodes1; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalSecondProblemTemperatureLaw(mesh.centerPoint(node),data);
		addToTemperatureField(node,temporaryTemperatureValue);
	}
}

AnalyticalSolution::AnalyticalSolution(TransientPlainWallInfo data):
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes,data.thermalConduction),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)
{
	this -> transientTemperatureField[0].resize(mesh.getNumberOfNodes());
	for (int i = 0; i < mesh.getNumberOfNodes(); ++i)
	{
		this -> transientTemperatureField[0][i] = data.initialTemperature;
	}
	
	vector<double> zetaNumbers(20); 
	getZetaNumbers(data.biotNumber, zetaNumbers);
	
	double timePosition = 0;
	int timeCounter = 0;
	double Tinf = this -> boundaries.getEndBoundaryCondition();
	double Tinit = data.initialTemperature;
	double alpha = data.thermalConduction/(data.density*data.cp);
	double diff = abs(this -> transientTemperatureField[timePosition][0] - Tinf);
	while (diff > 1)
	{		
		double fourierNumber = (alpha*timePosition)/(data.wallLength*data.wallLength);
		for (int positionCounter = 0; positionCounter < mesh.getNumberOfNodes(); ++positionCounter)
		{
			double sum = 0;
			for (int i = 0; i < zetaNumbers.size(); ++i)
			{
				double zeta = zetaNumbers[i];
				double cn = (4*sin(zeta))/(2*zeta+sin(2*zeta));
				sum += cn * exp(-zeta*zeta*fourierNumber) * cos(zeta*mesh.centerPoint(positionCounter)/mesh.getWallLength()); 
			}
			this -> transientTemperatureField[timeCounter][positionCounter] = Tinf + sum * (Tinit - Tinf);
		}
		
		diff = abs(this -> transientTemperatureField[timeCounter][0] - Tinf);
		timePosition += data.timeStep;
	}
}

AnalyticalSolution::AnalyticalSolution(PlainWallNonLinearInfo data):
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes,0.0),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());
	double temporaryTemperatureValue;
	for (int node = 0; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalNonLinearTemperatureLaw(mesh.centerPoint(node));
		addToTemperatureField(node,temporaryTemperatureValue);
	}
}

void AnalyticalSolution::writeSolutionToCsv(string directory, string fileName)
{
	string outputFileNameAndDirectory;
	outputFileNameAndDirectory = directory + "/" + fileName + ".csv";
	
	cout << "Analytical Solution results saved in " << outputFileNameAndDirectory << endl;
	ofstream pFile;
	pFile.open(outputFileNameAndDirectory,fstream::trunc);

	pFile << "nodesPosition, " << "nodesTemperature" << endl;
	pFile << fixed;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		pFile << mesh.centerPoint(i) << ", " << setprecision(20) << temperatureField[i] << endl;
	}
}

void AnalyticalSolution::printSolutionOnTheScreen()
{
	cout << "Analytical Solution" << endl;
	cout << setw(21) << "Position of the nodes, " << setw(24) << "Temperature of the Nodes" << endl;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		cout << setw(21) << mesh.centerPoint(i) << ", " << setw(24) << this -> temperatureField[i] << endl;
	}
}

void AnalyticalSolution::printTransientSolutionOnTheScreen()
{
	cout << "Analytical Solution" << endl;
	//cout << setw(21) << << "Position of the nodes, " << setw(24) << "Temperature of the Nodes" << endl;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		for (int j = 0; j < this -> transientTemperatureField.size(); ++j)
		{
			cout << i << ", " << mesh.centerPoint(i) << ", " << setw(24) << this -> transientTemperatureField[i][j] << endl;
		}
	}
}

double AnalyticalSolution::operator[](int index)
{
	return this -> temperatureField[index];

}

void AnalyticalSolution::addToTemperatureField(int index, double value)
{
	this -> temperatureField[index] = value;
}

double AnalyticalSolution::evalFirstProblemTemperatureLaw(double position)
{
	double beginPrescTemp = boundaries.getBeginBoundaryCondition();
	double endPrescTemp = boundaries.getEndBoundaryCondition();
	double wallLength = mesh.getWallLength();

	return beginPrescTemp + (endPrescTemp - beginPrescTemp)*(position/wallLength);
}

double AnalyticalSolution::evalSecondProblemTemperatureLaw(double position, DoublePlainWallInfo data)
{
	double Tinf = boundaries.getEndBoundaryCondition();
	double q = boundaries.getBeginBoundaryCondition();
	double h = boundaries.getEndConvectionCoeficient();
	double LA = data.wallLength1;
	double LB = data.wallLength2;
	double KA = data.thermalConduction1;
	double KB = data.thermalConduction2;
	double R = (LA/KA)+(LB/KB)+(1/h);
	double T0 = Tinf + q * R;
	if(position <= LA)
		return T0 - (q/KA)*position;
	else if ((position > LA) && (position <= LA + (1.01)*LB))
	{
		double TLA = T0 - (q/KA)*LA;
		R = (LA/KA) + (LB/KB);
		double TL = T0 - q * R;
		double d1 = -(h/KB) * (TL - Tinf);
		double d2 = TLA + (h/KB)*(TL - Tinf)*LA;
		return d2 + d1 * position;
	}
	else
	{
		cout << "Invalid position _ AN" << endl;
		return -100;
	}
}

double AnalyticalSolution::evalNonLinearTemperatureLaw(double x)
{
	//the output of this function is expressed in degrees celsius and the input must be in meters.
	double value;
	value = 160 - sqrt(3600 +160000 * x);
}

void AnalyticalSolution::getZetaNumbers(double biotNumber, vector<double> &zetaNumbers)
{
	for (int i = 0; i < zetaNumbers.size(); ++i)
	{
		zetaNumbers[i] = solve(biotNumber);
	}
}



double AnalyticalSolution::solve(double initialGuess)
{
	PetscErrorCode err;
	const PetscInt size = 1;

	Vec			y, x;
	Mat 		jacobian;
	SNES		snes;

	PetscInt solutionIndices[]={0};
	PetscScalar solution[1];

	/* Initial guess and solution vector */
	err = VecCreate(PETSC_COMM_WORLD, &x); CHKERRQ(err);
	err = PetscObjectSetName((PetscObject) x, "solution"); CHKERRQ(err);
	err = VecSetSizes(x, PETSC_DECIDE, size); CHKERRQ(err);
	err = VecSetFromOptions(x); CHKERRQ(err);
	err = VecDuplicate(x, &y); CHKERRQ(err);
	/* Initial guess */
	err = VecSet(x, (PetscScalar)initialGuess); CHKERRQ(err);
	err = PetscObjectSetName((PetscObject) y, "error"); CHKERRQ(err);

	/* Jacobian matrix */
	err = MatCreate(PETSC_COMM_WORLD, &jacobian); CHKERRQ(err);
	err = MatSetSizes(jacobian, PETSC_DECIDE, PETSC_DECIDE, 1, 1); CHKERRQ(err);
	err = MatSetFromOptions(jacobian); CHKERRQ(err);
	err = MatSetUp(jacobian); CHKERRQ(err);

	/* Create SNES context */
	err = SNESCreate(PETSC_COMM_WORLD, &snes); CHKERRQ(err);
	err = SNESSetFunction(snes, y, function, NULL); CHKERRQ(err);
	err = SNESSetJacobian(snes, jacobian, jacobian, dfunction, NULL); CHKERRQ(err);
	err = SNESSetFromOptions(snes); CHKERRQ(err);
	
	/* solve */
	err = SNESSolve(snes, NULL, x); CHKERRQ(err);

	/* Get solution */
	err = VecGetValues(x, size, solutionIndices, solution);

	/* Show solution */
	/*err = VecView(x, PETSC_VIEWER_STDOUT_WORLD);*/
	/*function(snes, x, y, NULL);*/
	/*err = VecView(y, PETSC_VIEWER_STDOUT_WORLD);*/

	/* Finalize */
	err = MatDestroy(&jacobian); CHKERRQ(err);
	err = VecDestroy(&x); CHKERRQ(err);
	err = VecDestroy(&y); CHKERRQ(err);
	err = SNESDestroy(&snes); CHKERRQ(err);

	return solution[0];
}

double AnalyticalSolution::calculateFunction(double x, double biotNumber)
{
	return x*tan(x) - biotNumber;
}

double AnalyticalSolution::calculateDerivate(double x)
{
	double sec, secSquare;
	sec = 1 / cos(x);
	secSquare = sec*sec;
	return tan(x) + x/secSquare;
}

PetscErrorCode AnalyticalSolution::function(SNES snes, Vec x, Vec f, void *ctx)
{
	PetscErrorCode err;
	const PetscScalar *xx;
	PetscScalar *ff;

	err = VecGetArrayRead(x, &xx); CHKERRQ(err);
	err = VecGetArray(f, &ff); CHKERRQ(err);

	ff[0] = (PetscScalar) calculateFunction((double) xx[0]);

	err = VecRestoreArrayRead(x, &xx); CHKERRQ(err);
	err = VecRestoreArray(f, &ff); CHKERRQ(err);
	
	return 0;
}

PetscErrorCode AnalyticalSolution::dfunction(SNES snes, Vec x, Mat jacobian, Mat B, void* ctx)
{
	PetscErrorCode err;
	const PetscInt size=1;
	const PetscInt rowEntry[]={0}, columnEntry[]={0};
	const PetscScalar *xx;
	PetscScalar df[1], sec, secSquare;

	err = VecGetArrayRead(x, &xx); CHKERRQ(err);

	sec = 1 / PetscCosReal(xx[0]);
	secSquare = sec*sec;
	df[0] = (PetscScalar) calculateDerivate((double) xx[0]);

	err = MatSetValues(jacobian, size, rowEntry, size, columnEntry, df, INSERT_VALUES); CHKERRQ(err);
	err = MatAssemblyBegin(jacobian, MAT_FINAL_ASSEMBLY); CHKERRQ(err);
	err = MatAssemblyEnd(jacobian, MAT_FINAL_ASSEMBLY); CHKERRQ(err);

	err = VecRestoreArrayRead(x, &xx); CHKERRQ(err);
	return 0;
}
