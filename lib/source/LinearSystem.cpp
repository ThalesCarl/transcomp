#include "LinearSystem.h"

LinearSystem::LinearSystem(int numberOfNodes): matrixOrder(numberOfNodes)
{	
	allocSolutionVector(numberOfNodes);
	allocIndependentValuesVector(numberOfNodes);
	allocMatrixOfCoeficients(numberOfNodes);
    
}

double LinearSystem::operator[](const int index)
{
	return this -> solution[index];

}

void LinearSystem::setValueToMatrix(int row, int col, double value)
{
	this -> matrixOfCoeficients[row][col] = value;

}
void LinearSystem::setValueToVector(int index, double value)
{
	this -> independentValues[index] = value;

}

void LinearSystem::solve()
{
    KSP            ksp;          /* linear solver context */
    Vec            x,b;          /* solution, residual vectors */
    Mat            A;            
    PetscInt n = this -> matrixOrder;
    PetscScalar value;      
    //PetscInt i = 0;

   
    /*----------------------------------------------------
        CRIAÇÃO DA MATRIZ A(COEFICIENTES LINEARES)
    -----------------------------------------------------*/
    MatCreate(PETSC_COMM_WORLD,&A);
    MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n);
    MatSetType(A,MATSEQAIJ);
    MatSetUp(A);
    
    for (int i = 0; i < n; i++)
    {
        for ( int j = 0; j < n; j++)
        {
            value = this -> matrixOfCoeficients[i][j];
            MatSetValues(A, 1, &i, 1, &j, &value, INSERT_VALUES);
        }
    }   
    MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
    
    /*---------------------------------------------
        CRIAÇÃO VETOR B (TERMOS INDEPENDENTES)
    ---------------------------------------------*/
    VecCreateSeq(PETSC_COMM_SELF, n, &b);
    for (int i = 0; i < n; i++)
    {
        value = this -> independentValues[i];
        VecSetValues(b,1,&i,&value,INSERT_VALUES);
    }
    VecAssemblyBegin(b);
    VecAssemblyEnd(b);

    /*---------------------------------------------
        CRIAÇÃO VETOR X (SOLUÇÃO)
    ---------------------------------------------*/
    VecCreateSeq(PETSC_COMM_WORLD, n, &x);
    VecSet(x,0);
    VecAssemblyBegin(b);
    VecAssemblyEnd(b);

    // MatView(A,PETSC_VIEWER_STDOUT_WORLD);
    // VecView(b,PETSC_VIEWER_STDOUT_WORLD);
    // VecView(x,PETSC_VIEWER_STDOUT_WORLD);

    /*---------------------------------------------
        CRIAÇÃO E EXECUÇÃO DO SOLVER
    ---------------------------------------------*/
    KSPCreate(PETSC_COMM_WORLD,&ksp);
    KSPSetOperators(ksp,A,A);
    KSPSetFromOptions(ksp);
    KSPSolve(ksp,b,x);
    
    for (int i = 0; i < n; i++)
    {
        VecGetValues(x,1,&i,&value);
        this -> solution[i] = value;
    }
    
    // VecView(x,PETSC_VIEWER_STDOUT_WORLD);
    // /*---------------------------------------------
    //     DESTRUIÇÃO DOS OBJETOS DA PETSC CRIADOS
    // ---------------------------------------------*/
    VecDestroy(&x);
    VecDestroy(&b);
    MatDestroy(&A);
    KSPDestroy(&ksp);
}

void LinearSystem::allocIndependentValuesVector(int vectorSize)
{
	this -> independentValues.resize(vectorSize);
    for(int i = 0; i < vectorSize; i++)
    {
        this -> independentValues[i]= 0.0;
    }
}

void LinearSystem::allocSolutionVector(int vectorSize)
{
	this -> solution.resize(vectorSize);
    for(int i = 0; i < vectorSize; i++)
    {
        this -> solution[i]= 0.0;
    }
}

void LinearSystem::allocMatrixOfCoeficients(int matrixSize)
{
	this -> matrixOfCoeficients.resize(matrixSize);
	for (int i = 0; i < matrixSize; i++)
	{
		matrixOfCoeficients[i].resize(matrixSize);
        for (int j = 0; j < matrixSize; j++)
        {
            this -> matrixOfCoeficients[i][j] = 0.0;
        }
	}
}

void LinearSystem::printValues()
{
    cout << "Matriz inserida:" << endl;
    for (int i = 0; i < this -> matrixOrder; i++)
    {
        for (int j = 0; j < this -> matrixOrder; j++)
        {
            
            cout << this -> matrixOfCoeficients[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Termos independentes inseridos:" << endl;
    for (int i = 0; i < this -> matrixOrder; i++)
    {
        cout << independentValues[i] << endl;
    }
}

void LinearSystem::printSolution()
{
    cout << "Printing the solution" << endl;
    for(int i= 0; i < this -> matrixOrder; i++)
    {
        cout << this -> solution[i] << endl;
    }
}

void LinearSystem::solveTDMA()
{
  double NaN;
  double DEN;
  NaN = nan("");

  vector<double> A;
  vector<double> B;
  vector<double> C;
  vector<double> D;

  A.resize(this->matrixOrder);
  B.resize(this->matrixOrder);
  C.resize(this->matrixOrder);
  D.resize(this->matrixOrder);

  A[0] = this -> matrixOfCoeficients[0][0];
  B[0] = this -> matrixOfCoeficients[0][1];
  C[0] = NaN;
  D[0] = this -> independentValues[0];
  for(int i=1; i<this->matrixOrder-1; i++)
  {
    A[i] = this -> matrixOfCoeficients[i][i];
    B[i] = this -> matrixOfCoeficients[i][i+1];
    C[i] = this -> matrixOfCoeficients[i][i-1];
    D[i] = this -> independentValues[i];
  }
  A[this->matrixOrder-1]=this -> matrixOfCoeficients[this -> matrixOrder-1][this -> matrixOrder - 1];
  B[this->matrixOrder-1]=NaN;
  C[this->matrixOrder-1]=this -> matrixOfCoeficients[this->matrixOrder-1][this->matrixOrder-2];
  D[this->matrixOrder-1]=this -> independentValues[this->matrixOrder-1];


  for(int i=1; i<matrixOrder; i++)
  {
    DEN=C[i]/A[i-1];
    A[i]=A[i]-DEN*B[i-1];
    D[i]=D[i]-DEN*D[i-1];
  }
  this->solution[matrixOrder-1]=D[matrixOrder-1]/A[matrixOrder-1];
  for(int i=matrixOrder-2; i>-1; i--)
  {
    this->solution[i]=(D[i]-B[i]*this->solution[i+1])/A[i];
  }
  return(this->CampodeTemperaturas);
}