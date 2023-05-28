#define verts_amount 11
#include <stdlib.h>
#include <stdio.h>
#include "props.h"

double **randm(int n) {
    srand(2211);
    double **matrix = (double **) malloc(sizeof(double *) * n);
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *) malloc(sizeof(double) * n);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double) (rand() * 2.0) / (double) RAND_MAX;
        }
    }
    return matrix;

}

double **mulmr(double coef, double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= coef;
            matrix[i][j] = matrix[i][j] < 1 ? 0 : 1;
        }
    }
    return matrix;
}

double **symmetricalMatrix(double **matrix, int n) {
    double **symmetrical = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; ++i) {
        symmetrical[i] = (double *) malloc(n * sizeof(double));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            symmetrical[i][j] = matrix[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (symmetrical[i][j] != symmetrical[j][i]) {
                symmetrical[i][j] = 1;
                symmetrical[j][i] = 1;
            }
        }
    }
    return symmetrical;
}

void freeMatrix(double **matrix, int n) {
    for (int i = 0; i < n; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

int* graphDegrees(double** matrix) {
    const int number = verts_amount;
    int* vertexDegree;
    vertexDegree = malloc(number * sizeof(int));
    int vertexDegreeCounter;
    for (int i = 0; i < number; ++i) {
        vertexDegreeCounter = 0;
        for (int j = 0; j < number; ++j) {
            if(matrix[i][j] && i == j) vertexDegreeCounter += 2;
            else if(matrix[i][j]) vertexDegreeCounter++;
        }
        vertexDegree[i] = vertexDegreeCounter;
    }
    return vertexDegree;
}

int* halfDegreeEntry(double** matrix) {
    const int number = verts_amount;
    int* vertexDegree;
    vertexDegree = malloc(number * sizeof(int));
    int vertexDegreeCounter;
    for (int j = 0; j < number; ++j) {
        vertexDegreeCounter = 0;
        for (int i = 0; i < number; ++i) {
            if(matrix[i][j] ) vertexDegreeCounter++;
        }
        vertexDegree[j] = vertexDegreeCounter;
    }
    return vertexDegree;
}

int* halfDegreeExit(double** matrix) {
    const int number = verts_amount;
    int* vertexDegree;
    vertexDegree = malloc(number * sizeof(int));
    int vertexDegreeCounter;
    for (int i = 0; i < number; ++i) {
        vertexDegreeCounter = 0;
        for (int j = 0; j < number; ++j) {
            if(matrix[i][j]) vertexDegreeCounter++;
        }
        vertexDegree[i] = vertexDegreeCounter;
    }
    return vertexDegree;
}

int* summarizeHalfDegrees(const int* exit, const int* entry) {
    const int number = verts_amount;
    int* vertexDegree = malloc(number * sizeof(int));
    for (int i = 0; i < number; ++i) {
        vertexDegree[i] = exit[i] + entry[i];
    }

    return vertexDegree;
}

int isUniform(const int* degreesArray) {
    const int number = verts_amount;
    int firstDegree = degreesArray[0];
    for (int i = 1; i < number; ++i) {
        if(degreesArray[i] != firstDegree) return 0;
        firstDegree = degreesArray[i];
    }
    return 1;
}

double** summarizeMatrices(double** AMatrix, double** BMatrix) {
    const int number = verts_amount;
    double **summedMatrix = (double **) malloc(sizeof(double *) * number);
    for (int i = 0; i < number; i++) {
        summedMatrix[i] = (double *) malloc(sizeof(double) * number);
        for (int j = 0; j < number; ++j) {
            summedMatrix[i][j] = AMatrix[i][j] + BMatrix[i][j];
        }
    }
    return summedMatrix;
}

double** multiplyMatrices(double** AMatrix, double** BMatrix) {
    const int number = verts_amount;
    double **multipliedMatrix = (double **) malloc(sizeof(double *) * number);
    for (int i = 0; i < number; ++i) {
        multipliedMatrix[i] = (double *) malloc(sizeof(double) * number);
        for (int j = 0; j < number; ++j) {
            multipliedMatrix[i][j] = 0;
            for (int e = 0; e < number; ++e) {
                multipliedMatrix[i][j] += AMatrix[i][e] * BMatrix[e][j];
            }
        }
    }
    return multipliedMatrix;
}

double** copyMatrix(double** matrix) {
    const int number = verts_amount;
    double **copiedMatrix = (double **) malloc(sizeof(double *) * number);
    for (int i = 0; i < number; ++i) {
        copiedMatrix[i] = (double *) malloc(sizeof(double) * number);
        for (int j = 0; j < number; ++j) {
            copiedMatrix[i][j] = matrix[i][j];
        }
    }
    return copiedMatrix;
}

void booleanConversion(double** matrix) {
    const int number = verts_amount;
    for (int i = 0; i < number; ++i) {
        for (int j = 0; j < number; ++j) {
            if (matrix[i][j]) matrix[i][j] = 1;
        }
    }
}

double **directedMatrix(double K) {
    double **T = randm(verts_amount);
    double **A = mulmr(K, T, verts_amount);
    return A;
}

double** getReachabilityMatrix(double** matrix) {
    const int number = verts_amount;
    double **copy = copyMatrix(matrix);
    double **sum = copy;
    double **prev = copy;
    double **tempPrev, **tempSum;
    for (int i = 1; i < number - 1; i++) {
        tempPrev = multiplyMatrices(prev, matrix);
        tempSum = summarizeMatrices(sum, tempPrev);
        freeMatrix(sum,number);
        freeMatrix(prev,number);
        prev = tempPrev;
        sum = tempSum;
    }
    for (int i = 0; i < number; i++) {
        sum[i][i] += 1;
    }
    freeMatrix(prev,number);
    booleanConversion(sum);
    return sum;
}

void depthFirstSearch(double** connectivityMatrix, int startVertex, double* component, int* visited) {
    const int number = verts_amount;
    visited[startVertex] = 1;
    component[startVertex] = 1;
    for (int nearbyVertex = 0; nearbyVertex < number; ++nearbyVertex) {
        if(!visited[nearbyVertex] && connectivityMatrix[startVertex][nearbyVertex]) {
            depthFirstSearch(connectivityMatrix, nearbyVertex, component, visited);
        }
    }
}

double** transposeMatrix(double** matrix,  int number) {
    number = verts_amount;
    double **transposedMatrix = malloc(number * sizeof(double*));
    for (int i = 0; i < number; ++i) {
        transposedMatrix[i] = malloc(number * sizeof(double ));
        for (int j = 0; j < number; ++j) {
            transposedMatrix[i][j] = matrix[j][i];
        }
    }
    return transposedMatrix;

}

int countNonZeroEntries(double **matrix) {
    int numVertices = verts_amount;
    int count = 0;

    for (int row = 0; row < numVertices; row++) {
        for (int col = 0; col < numVertices; col++) {
            if (matrix[row][col]) {
                count++;
                row++;
            }
        }
    }
    return count;
}

double** findStrongComponents(double** strongMatrix) {
    const int number = verts_amount;
    int* visitedVertex = calloc(number, sizeof(int));
    double** connectedComponents = calloc(number, sizeof(double *));
    for (int i = 0; i < number; i++) {
        connectedComponents[i] = calloc(number, sizeof(double));
    }

    for (int i = 0; i < number; ++i) {
        if(!visitedVertex[i]) {
            depthFirstSearch(strongMatrix, i, connectedComponents[i], visitedVertex);
        }
    }

    free(visitedVertex);
    return connectedComponents;
}

double** getStrongConnectivityMatrix(double **reachabilityMatrix) {
    const int number = verts_amount;
    double** transposedMatrix = transposeMatrix(reachabilityMatrix, number);
    double **strongConnectivityMatrix = malloc(number* sizeof(double*));
    for (int i = 0; i < number; i++) {
        strongConnectivityMatrix[i] = malloc(number* sizeof(double));
        for (int j = 0; j < number; j++) {
            strongConnectivityMatrix[i][j] = reachabilityMatrix[i][j] * transposedMatrix[i][j];
        }
    }
    freeMatrix(transposedMatrix, number);
    return strongConnectivityMatrix;
}

void condensationMatrix(double** strongComponents) {
    int numComponents = countNonZeroEntries(strongComponents);

    double **adjacencyMatrix = calloc(numComponents, sizeof(double*));
    for (int i = 0; i < numComponents; i++) {
        adjacencyMatrix[i] = calloc(numComponents, sizeof(double));
    }

    int position = 1;
    for (int i = 0; i < verts_amount; ++i) {
        if(!strongComponents[0][i]) {
            adjacencyMatrix[0][position] = 1;
            position++;
        }
    }

    for (int i = 0; i < numComponents; i++) {
        for (int j = 0; j < numComponents; j++) {
            printf("%.0lf  ", adjacencyMatrix[i][j]);
        }
        printf("\n");
    }

    freeMatrix(adjacencyMatrix, verts_amount);

}
double **generateAdjacencyMatrixFromStrongComponents(double **components) {
    const int number = verts_amount;
    double **matrix = calloc(number, sizeof(size_t*));
    for (int i = 0; i < number; i++) {
        matrix[i] = calloc(number, sizeof(double));
    }

    for(int i = 0; i < number; i++) {
        if (!components[0][i]) matrix[1][i+1] = 1;
    }

    return matrix;
}

void dfss(double** strongMatrix, int vertex, int* visitedVertex) {
    visitedVertex[vertex] = 1;

    for (int i = 0; i < verts_amount; ++i) {
        if (strongMatrix[vertex][i] && !visitedVertex[i]) {
            dfss(strongMatrix, i, visitedVertex);
        }
    }
}

double **condensationMatrixWithCoefficient(double K) {
    double **matrix = directedMatrix(K);
    double **reachability = getReachabilityMatrix(matrix);
    double **connectivity = getStrongConnectivityMatrix(reachability);
    double **components = findStrongComponents(connectivity);

    freeMatrix(matrix, verts_amount);
    freeMatrix(reachability, verts_amount);
    freeMatrix(connectivity, verts_amount);
    return components;
}

int getStrongComponents(double** strongMatrix) {
    const int number = verts_amount;
    int* visitedVertex = calloc(number, sizeof(int));
    int count = 0;

    for (int i = 0; i < number; ++i) {
        if (!visitedVertex[i]) {
            dfss(strongMatrix, i,  visitedVertex);
            count++;
        }
    }

    free(visitedVertex);
    return count;
}