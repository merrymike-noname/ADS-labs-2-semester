#include <stdio.h>
#include <stdlib.h>
#define verts_amount 11
#include "props.h"

void typeMatrix(double **matrix) {
    const int number = verts_amount;
    for (int i = 0; i < number; i++) {
        for (int j = 0; j < number; j++) {
            printf("%.0lf  ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printComponents(double **matrix, int number) {
    int n = verts_amount;
    int componentCount = 1;
    for (int i = 0; i < n; i++) {
        int isNewComponent = 1;
        for (int j = 0; j < n; j++) {
            if(matrix[i][j]) {
                if (isNewComponent) printf("Component %d: [  ", componentCount);
                printf("%d  ", j + 1);
                isNewComponent = 0;
            }
        }
        if (!isNewComponent) {
            componentCount++;
            printf("]\n");
        }
    }
}

void printDegrees(int *degrees) {
    const int number = verts_amount;
    printf("{  ");
    for (int i = 0; i < number; i++) {
        printf("%d  ", degrees[i]);
    }
    printf("}\n");
}

int getInterimVertsLength2(double** modifiedMatrix, int startPoint, int endPoint){
    int interimVert;
    for (int i = 0; i < verts_amount; ++i) {
        for (int j = 0; j < verts_amount; ++j) {
            if (modifiedMatrix[startPoint][j] == 1 && modifiedMatrix[j][endPoint] == 1)
                interimVert = j;
        }
    }
    return ++interimVert;
}

int* getInterimVertsLength3(double** modifiedMatrix, double** squaredMatrix, int startPoint, int endPoint){
    int* interimVerts = calloc(2, sizeof(int));
    for (int i = 0; i < verts_amount; ++i) {
        for (int j = 0; j < verts_amount; ++j) {
            if (modifiedMatrix[startPoint][j] == 1 && squaredMatrix[j][endPoint] >= 1){
                interimVerts[0] = j;
                for (int k = 0; k < verts_amount; ++k) {
                    if (modifiedMatrix[interimVerts[0]][k] == 1 && modifiedMatrix[k][endPoint]){
                        interimVerts[1] = k;
                    }
                }
            }
        }
    }
    interimVerts[0] += 1;
    interimVerts[1] += 1;
    return interimVerts;
}

void printPathwaysLength2(double** pathMatrix, double** modifiedMatrix) {
    const int numbers = verts_amount;
    for (int i = 0; i < numbers; i++) {
        for (int j = 0; j < numbers; j++) {
            if (*(*(pathMatrix + i) + j) != 0) {
                int interimVert = getInterimVertsLength2(modifiedMatrix, i, j);
                printf("%d -> %d -> %d; ", i + 1, interimVert, j + 1);
            }
        }
        printf("\n");
    }
}

void printPathwaysLength3(double** pathMatrix, double** modifiedMatrix, double** squaredMatrix) {
    const int numbers = verts_amount;
    for (int i = 0; i < numbers; i++) {
        for (int j = 0; j < numbers; j++) {
            if (*(*(pathMatrix + i) + j) != 0) {
                int* interimVerts = getInterimVertsLength3(modifiedMatrix, squaredMatrix, i, j);
                printf("%d -> %d -> %d -> %d; ", i + 1, interimVerts[0], interimVerts[1], j + 1);
                free(interimVerts);
            }
        }
        printf("\n");
    }
}

void printVertices(int* verticesNumber) {
    printf("{ ");
    for (int i = 0; verticesNumber[i] != 0; ++i) {
        printf("%d ", verticesNumber[i]);
    }
    printf("}\n");
}

void directedGraphInfo() {
    double **T = randm(verts_amount);
    double coefficient = 1.0 - 0.01 - 0.01 - 0.3;
    double **A = mulmr(coefficient, T, verts_amount);
    int* entry = halfDegreeEntry(A);
    int* exit = halfDegreeExit(A);
    int* summedDegrees = summarizeHalfDegrees(exit, entry);

    printf("\n\nDirected Graph \n");
    printf("\n\tInitial matrix\n");
    typeMatrix(A);

    printf("Exit degree : ");
    printDegrees(exit);

    printf("Entry degree : ");
    printDegrees(entry);

    if(isUniform(summedDegrees)) {
        printf("%d\n", summedDegrees[0]);
    } else {
        printf("\tThe graph is not uniform ");
    }

    printf("\nIsolated vertices : ");

    for (int i = 0; i < verts_amount; ++i) {
        if (summedDegrees[i] == 0) printf("# %d ", i+1);
    }

    printf("\nTerminal vertices : ");
    for (int i = 0; i < verts_amount; ++i) {
        if (summedDegrees[i] == 1) printf("# %d ", i+1);
    }
    freeMatrix(A, verts_amount);

    free(entry);
    free(exit);

    free(summedDegrees);
}

void undirectedGraphInfo() {
    double coefficient = 1.0 - 0.01 - 0.01 - 0.3;
    double **R = randm(verts_amount);
    double **C = symmetricalMatrix(mulmr(coefficient, R, verts_amount), verts_amount);
    int* degree = graphDegrees(C);

    printf("\n\nUndirected Graph \n");
    printf("\n\tMatrix for Undirected Graph\n");
    typeMatrix(C);

    printf("Undirected graph degrees : ");
    printDegrees(degree);

    if(isUniform(degree)) {
        printf("%d\n", degree[0]);
    } else {
        printf("\tThe graph is not uniform ");
    }

    printf("\nIsolated vertices : ");
    for (int i = 0; i < verts_amount; ++i) {
        if (degree[i] == 0) printf("# %d ", i+1);
    }

    printf("\nTerminal vertices : ");
    for (int i = 0; i < verts_amount; ++i) {
        if (degree[i] == 1) printf("# %d ", i+1);
    }

    freeMatrix(C, verts_amount);
    free(degree);
}

void modifiedGraphInfo() {
    double** K = randm(verts_amount);
    double modifiedCoefficient = 1.0 - 0.005 - 0.005 - 0.27;
    double** D = mulmr(modifiedCoefficient, K, verts_amount);
    int* entry = halfDegreeEntry(D);
    int* exit = halfDegreeExit(D);
    int* summedDegrees = summarizeHalfDegrees(exit, entry);
    double** squaredMatrix = multiplyMatrices(D,D);
    double** cubedMatrix = multiplyMatrices(squaredMatrix, D);
    double **reachabilityMatrix = getReachabilityMatrix(D);
    double **connectivityMatrix = getStrongConnectivityMatrix(reachabilityMatrix);

    double** strongComponents = findStrongComponents(connectivityMatrix);
    printf("\n\nModified Graph \n");
    printf("\n\tMatrix for Modified Graph\n");
    typeMatrix(D);

    printf("Exit degree : ");
    printDegrees(exit);

    printf("Entry degree : ");
    printDegrees(entry);

    if(isUniform(summedDegrees)) {
        printf("%d\n", summedDegrees[0]);
    } else {
        printf("\tThe graph is not uniform ");
    }

    printf("\nIsolated vertices : ");
    for (int i = 0; i < verts_amount; ++i) {
        if (summedDegrees[i] == 0) printf("# %d ", i+1);
    }

    printf("\nTerminal vertices : ");
    for (int i = 0; i < verts_amount; ++i) {
        if (summedDegrees[i] == 1) printf("# %d ", i+1);
    }

    printf("\n\nMatrix squared : 2\n");
    typeMatrix(squaredMatrix);

    printf("\nPathways with length : 2\n");
    printPathwaysLength2(squaredMatrix, D);

    printf("\nMatrix cubed : 3\n");
    typeMatrix(cubedMatrix);

    printf("\nPathways with length : 3\n");
    printPathwaysLength3(cubedMatrix, D, squaredMatrix);

    printf("\nReachability Matrix of Mod graph\n");
    typeMatrix(reachabilityMatrix);

    printf("\nConnected Matrix of Mod graph\n");
    typeMatrix(connectivityMatrix);

    printf("\nStrongly Connected Components of Mod Graph\n");
    printComponents(strongComponents, verts_amount);

    printf("\nMatrix of Condensation Graph\n");
    condensationMatrix(strongComponents);

    freeMatrix(D, verts_amount);
    freeMatrix(squaredMatrix, verts_amount);
    freeMatrix(cubedMatrix, verts_amount);
    freeMatrix(reachabilityMatrix, verts_amount);
    freeMatrix(connectivityMatrix, verts_amount);
    freeMatrix(strongComponents, verts_amount);
    free(summedDegrees);
    free(entry);
    free(exit);
}

int main() {
    directedGraphInfo();
    undirectedGraphInfo();
    modifiedGraphInfo();
}
