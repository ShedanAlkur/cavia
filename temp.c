#include <stdio.h>
#include <stdlib.h>

void allocMatrix(int ***ppMtx, int nRow, int nCol) {
    *ppMtx = malloc(nRow * sizeof **ppMtx);
    int *data = malloc(nRow * nCol * sizeof *data);
    for (int i = 0; i < nRow; ++i) (*ppMtx)[i] = data + i * nCol;
}

struct Graph {
    int **arcs;
    int *visitedV;
    int *trackNumber;
    int vertexCount;
    int startVertex;
    int endVertex;
};

void *search(struct Graph *graph, int vertex, int *parentPath,
                  int parentPathLength) {
    // дополнение пути текущей вершиной
    int pathLength = parentPathLength + 1;
    int *path = malloc((*graph).vertexCount * sizeof *path);
    for (int i = 0; i < (*graph).vertexCount; ++i) path[i] = parentPath[i];
    path[pathLength - 1] = vertex;
    (*graph).visitedV[vertex] = 1;

    // если нашли конечную вершину, возращаем найденный путь
    if (vertex == (*graph).endVertex) return path;

    // подсчет доступных путей
    int trackCount = 0;
    for (int i = 0; i < (*graph).vertexCount; ++i)
        trackCount += ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) != 0;

    do {
        trackCount = 0;
        for (int i = 0; i < (*graph).vertexCount; ++i)
            trackCount +=
                ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) != 0;

        for (int i = 0; i < (*graph).vertexCount; ++i)
            if ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) {
                vertex = i;
                pathLength += 1;
                path[pathLength] = i;
            }
    } while (trackCount != 1);

    switch (trackCount) {
        case 0:  // если пути нет, возвращаем ошибку
            free(path);
            return -1;
            break;
        case 1:  // если путь один, функция переходит по нему
            for (int i = 0; i < (*graph).vertexCount; ++i)
                if ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) {
                    vertex = i;
                    pathLength += 1;
                    path[pathLength] = i;
                }
            break;
        default:  // если путей несколько, функция запускает дочерние функции
                  // поиска по ним
            for (int i = 0; i < (*graph).vertexCount; ++i)
                if ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) {
                    void *result = search(graph, i, path, pathLength);
                    free(path);
                    return result;
                }
            break;
    }

    return 0;
}

int main() {
    FILE *input = fopen("input.txt", "r");

    if (!input) {
        printf("Error: Filename \"input.txt\" not found!\n");
    }

    int N = 0;
    fscanf(input, "%d", &N);
    printf("N = %d;\n", N);

    int *trackNumber = malloc(N * sizeof *trackNumber);
    for (int i = 0; i < N; ++i) trackNumber[i] = 0;

    printf("Arcs:");
    int **arcs = 0;
    allocMatrix(&arcs, N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fscanf(input, "%d", &arcs[i][j]);
            printf(" %d", arcs[i][j]);
            if (arcs[i][j]) trackNumber[i] += 1;
        }
    }
    printf(";\n");

    int VS = 0, VE = 0;
    fscanf(input, "%d%d", &VS, &VE);
    printf("VS = %d, VE = %d.\n\n", VS, VE);

    int *visitedV = malloc(N * sizeof *visitedV);
    for (int i = 0; i < N; ++i) visitedV[i] = 0;

    struct Graph graph;
    graph.arcs = arcs;
    graph.visitedV = visitedV;
    graph.vertexCount = N;
    graph.startVertex = VS;
    graph.endVertex = VE;
    graph.trackNumber = trackNumber;

    int *path = malloc(N * sizeof *path);
    search(&graph, VS, path, 0);
    free(path);

    free(arcs[0]);
    free(arcs);
    free(visitedV);

    return EXIT_SUCCESS;
}
