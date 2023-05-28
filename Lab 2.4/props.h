#ifndef LAB_2_4_PROPS_H
#define LAB_2_4_PROPS_H

double **randm(int n);
double **mulmr(double coef, double **matrix, int n);
int* halfDegreeEntry(double** matrix);
int* halfDegreeExit(double** matrix);
int* summarizeHalfDegrees(const int* exit, const int* entry);
int isUniform(const int* degreesArray);
void freeMatrix(double **matrix, int n);
double **symmetricalMatrix(double **matrix, int n);
int* graphDegrees(double** matrix);
double** multiplyMatrices(double** AMatrix, double** BMatrix);
double** getStrongConnectivityMatrix(double **reachabilityMatrix);
double** getReachabilityMatrix(double** matrix);
double** findStrongComponents(double** strongMatrix);
void condensationMatrix(double** strongComponents);
double **generateAdjacencyMatrixFromStrongComponents(double **components);
double **condensationMatrixWithCoefficient(double K);
int getStrongComponents(double** strongMatrix);
#endif