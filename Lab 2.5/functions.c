#include <stdlib.h>
#define verts_amount 11

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

void fillZero(double** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = 0.;
        }
    }
}

double** createMatrix(int n) {
  double **matrix = (double **) malloc(n * sizeof(double *));
  for (int i = 0; i < n; i++) {
    matrix[i] = (double *) malloc(n * sizeof(double));
  }
  return matrix;
}

void freeMatrix(double **matrix, int n) {
  for (int i = 0; i < n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

double** createCrossingMatrix(const int* array) {
    double** traversalMatrix = createMatrix(verts_amount);
    fillZero(traversalMatrix, verts_amount);
    for (int i = 0; i < verts_amount; i++)
    {
        traversalMatrix[array[i]][i] = 1.0;
    }
    return traversalMatrix;
}

void buildSearchMatrix(double** graph, int sourceVertex, double** searchMatrix) {
    const int number = verts_amount;
    for (int i = 0; i < number; ++i) {
        if (graph[i][sourceVertex] == 1) searchMatrix[i][sourceVertex] = 1;
    }
}

int findFirst(double** matrix, int n) {
  int* outgoingCounts = (int*)calloc(n, sizeof(int));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (matrix[i][j] == 1) {
        outgoingCounts[i]++;
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    if (outgoingCounts[i] > 0) {
      free(outgoingCounts);
      return i;
    }
  }
  free(outgoingCounts);
  return -1;
}

void breadthFirstSearch(double** adjacencyMatrix, int startVertex, int* queue, double** tree) {
    const int number = verts_amount;
    int visited[number];
    for (int i = 0; i < number; i++)
    {
        visited[i] = 0;
    }
    int queueStart = 0;
    int queueFinish = 0;

    visited[startVertex] = 1;
    queue[queueFinish] = startVertex;

    while (queueStart <= queueFinish) {
        int currentVertex = queue[queueStart++];
        for (int neighborVertex = 0; neighborVertex < number; neighborVertex++) {
            if (adjacencyMatrix[currentVertex][neighborVertex] == 1 && visited[neighborVertex] == 0)
            {
                tree[currentVertex][neighborVertex] = 1;
                queue[++queueFinish] = neighborVertex;
                visited[neighborVertex] = 1;
            }
        }
    }
    for (int i = 0; i < number; ++i) {
        if (visited[i] == 0){
            int currentVertex = i;
            for (int neighborVertex = 0; neighborVertex < number; neighborVertex++) {
                if (adjacencyMatrix[currentVertex][neighborVertex] == 1 && visited[neighborVertex] == 0 && currentVertex != neighborVertex)
                {
                    queue[++queueFinish] = currentVertex;
                    tree[currentVertex][neighborVertex] = 1;
                    queue[++queueFinish] = neighborVertex;
                    visited[neighborVertex] = 1;
                }
            }
        }
    }
}

void depthFirstSearch(double** adjacencyMatrix, int currentVertex, int* visited, int* depthVertices, double** tree, int* numVisited) {
    const int number = verts_amount;
    visited[currentVertex] = 1;
    depthVertices[(*numVisited)] = currentVertex;
    (*numVisited)++;
    for (int neighborVertex = 0; neighborVertex < number; ++neighborVertex) {
        if (adjacencyMatrix[currentVertex][neighborVertex] == 1 && visited[neighborVertex] == 0) {
            tree[currentVertex][neighborVertex] = 1;
            depthFirstSearch(adjacencyMatrix, neighborVertex, visited, depthVertices, tree, numVisited);
        }
    }
}

void runDfsForNotVisitedVertices(int currentVertex, double** adjacencyMatrix,
                                 int* visited, int amount, int* depthVertices, double** graph ) {
  const int number = verts_amount;
  for (int i = 0; i < number; ++i) {
    visited[i] = 0;
  }
  for (int i = 0; i < number; ++i) {
    if (visited[i] == 0) {
      depthFirstSearch(adjacencyMatrix, i, visited, depthVertices,graph, &amount);
    }
  }
}