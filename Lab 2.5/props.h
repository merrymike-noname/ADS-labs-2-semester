#ifndef LAB_2_5_PROPS_H
#define LAB_2_5_PROPS_H
double **randm(int n);
double **mulmr(double coef, double **matrix, int n);
void fillZero(double** matrix, int n);
double** createMatrix(int n);
void freeMatrix(double **matrix, int n);
double** createCrossingMatrix(const int* array);
void buildSearchMatrix(double** graph, int sourceVertex, double** searchMatrix);
int findFirst(double** matrix, int n);
void breadthFirstSearch(double** adjacencyMatrix, int startVertex, int* queue, double** tree);
void depthFirstSearch(double** adjacencyMatrix, int currentVertex, int* visited, int* depthVertices, double** tree, int* numVisited);
void runDfsForNotVisitedVertices(int currentVertex, double** adjacencyMatrix, int* visited, int amount, int* depthVertices, double** graph );
#endif
