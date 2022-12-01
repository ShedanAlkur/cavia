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
    // копирование пути родителя
    int *path = malloc((*graph).vertexCount * sizeof *path);
    for (int i = 0; i < (*graph).vertexCount; ++i) path[i] = parentPath[i];

    // дополнение пути текущей вершиной
    int pathLength = parentPathLength + 1;
    path[pathLength - 1] = vertex;
    // обновление структуры графа
    (*graph).visitedV[vertex] = 1;
    for (int i = 0; i < (*graph).vertexCount; ++i)
        if ((*graph).arcs[i][vertex]) (*graph).trackNumber[i] -= 1;

    // если найдена конечная вершина, возвращаем найденный путь
    if (vertex == (*graph).endVertex) return path;

    // если возможен переход только в одну вершину, переходим в неё
    while ((*graph).trackNumber[vertex] == 1) {
        for (int i = 0; i < (*graph).vertexCount; ++i)
            if ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) {
                vertex = i;
                pathLength += 1;
                path[pathLength - 1] = vertex;

                (*graph).visitedV[vertex] = 1;

                for (int i = 0; i < (*graph).vertexCount; ++i)
                    if ((*graph).arcs[i][vertex]) (*graph).trackNumber[i] -= 1;
                if (vertex == (*graph).endVertex) return path;
            }
    }

    // если переход невозможен, возвращаем сообщение об ошибке
    if ((*graph).trackNumber[vertex] == 0) {
        *path = -1;
        return path;
    }

    // если переход возможен в несколько вершин, запускаем в них дочернии
    // функции поиска
    for (int i = 0; i < (*graph).vertexCount; ++i)
        if ((*graph).arcs[vertex][i] && !(*graph).visitedV[i]) {
            int *result = search(graph, i, path, pathLength);
            if (*result != -1) return result;
            free(result);
        }

    // если путь не найден, возвращаем сообщение об ошибке
    *path = -1;
    return path;
}

int main() {
    // открытие файла
    FILE *input = fopen("input.txt", "r");

    if (!input) {
        printf("Error: Filename \"input.txt\" not found!\n");
    }

    struct Graph graph;

    // чтение количества вершин в графе
    int N = 0;
    fscanf(input, "%d", &N);
    graph.vertexCount = N;
    printf("N = %d;\n", N);

    // список полустепеней исхода вершин
    graph.trackNumber = malloc(N * sizeof *graph.trackNumber);
    for (int i = 0; i < N; ++i) graph.trackNumber[i] = 0;

    // чтение информациио дугах графа
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

    // чтение начальной и конечной вершины для поиска
    fscanf(input, "%d%d", &graph.startVertex, &graph.endVertex);
    printf("VS = %d, VE = %d.\n\n", graph.startVertex, graph.endVertex);

    // закрытие файла
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

    return EXIT_SUCCESS;
}
