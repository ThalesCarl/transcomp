#include <stdlib.h>
#include <stdio.h>
#include <petscsnes.h>

#define BI 2.2

double calculateFunction(double x);
double calculateDerivate(double x);
PetscErrorCode function(SNES snes, Vec x, Vec f, void *ctx);
PetscErrorCode dfunction(SNES snes, Vec x, Mat A, Mat B, void* ctx);
double solve(double initialGuess);

double solve(double initialGuess)
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

double calculateFunction(double x)
{
	return x*tan(x) - BI;
}

double calculateDerivate(double x)
{
	double sec, secSquare;
	sec = 1 / cos(x);
	secSquare = sec*sec;
	return tan(x) + x/secSquare;
}

PetscErrorCode function(SNES snes, Vec x, Vec f, void *ctx)
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

PetscErrorCode dfunction(SNES snes, Vec x, Mat jacobian, Mat B, void* ctx)
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
