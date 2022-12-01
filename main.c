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

int *search(struct Graph *graph, int vertex, int *parentPath,
            int parentPathLength) {
    // дополнение пути текущей вершиной
    int pathLength = parentPathLength + 1;
    int *path = malloc((*graph).vertexCount * sizeof *path);
    for (int i = 0; i < (*graph).vertexCount; ++i) path[i] = parentPath[i];
    path[pathLength - 1] = vertex;
    (*graph).visitedV[vertex] = 1;

    for (int i = 0; i < (*graph).vertexCount; ++i)
        if ((*graph).arcs[i][vertex]) (*graph).trackNumber[i] -= 1;

    // если нашли конечную вершину, возращаем найденный путь
    if (vertex == (*graph).endVertex) return path;

    while ((*graph).trackNumber[vertex] == 1) {
        for (int i = 0; i < (*graph).vertexCount; ++i)
            if ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) {
                vertex = i;
                pathLength += 1;
                path[pathLength - 1] = vertex;
                (*graph).visitedV[vertex] = 1;

                for (int i = 0; i < (*graph).vertexCount; ++i)
                    if ((*graph).arcs[i][vertex]) (*graph).trackNumber[i] -= 1;

                // return!!!
                if (vertex == (*graph).endVertex) return path;
            }
    }

    if ((*graph).trackNumber[vertex] == 0) {
        // return!!!
        *path = -1;
        return path;
    }

    for (int i = 0; i < (*graph).vertexCount; ++i)
        if ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) {
            // return!!!
            int *result = search(graph, i, path, pathLength);
            if (*result != -1) return result;
            free(result);
        }

    // return!!!
    *path = -1;
    return path;
}

int main() {
    FILE *input = fopen("input.txt", "r");

    if (!input) {
        printf("Error: Filename \"input.txt\" not found!\n");
    }

    struct Graph graph;

    int N = 0;
    fscanf(input, "%d", &N);
    graph.vertexCount = N;
    printf("N = %d;\n", N);

    graph.trackNumber = malloc(N * sizeof *graph.trackNumber);
    for (int i = 0; i < N; ++i) graph.trackNumber[i] = 0;

    printf("Arcs:");
    graph.arcs = 0;
    allocMatrix(&graph.arcs, N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fscanf(input, "%d", &graph.arcs[i][j]);
            printf(" %d", graph.arcs[i][j]);
            if (graph.arcs[i][j]) graph.trackNumber[i] += 1;
        }
        printf(",");
    }
    printf(";\n");

    fscanf(input, "%d%d", &graph.startVertex, &graph.endVertex);
    printf("VS = %d, VE = %d.\n\n", graph.startVertex, graph.endVertex);

    fclose(input);

    graph.visitedV = malloc(N * sizeof *graph.visitedV);
    for (int i = 0; i < N; ++i) graph.visitedV[i] = 0;

    int *path = malloc(N * sizeof *path);
    for (int i = 0; i < N; ++i) path[i] = -1;
    int *result = search(&graph, graph.startVertex, path, 0);

    if (*result == -1) {
        printf("Path not found.\n");
    } else {
        printf("Path found: ");
        for (int i = 0; i < N; ++i) {
            if ((result)[i] == -1) break;
            printf(" %d", (result)[i]);
        }
        printf("\n");
    }

    free(path);
    free(graph.arcs[0]);
    free(graph.arcs);
    free(graph.visitedV);
    free(result);
    // TODO: Освободить ресурсы!

    return EXIT_SUCCESS;
}
