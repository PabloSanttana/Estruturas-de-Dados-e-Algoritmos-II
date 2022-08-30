#pragma once
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class GrafoListaAdj {
private:
    vector<string> vertices;

    //first é o indice do vertice, second é o peso (caso o grafo seja ponderado)
    vector<vector<pair<int, int>>> arestas;

    /**
    * A principio nao temos nenhuma ordenacao usando os rotulos.
    * Portanto, usaremos busca linear.
    **/
    int obterIndiceVertice(string rotuloVertice) {
        //IMPLEMENTAR
        size_t tam = vertices.size();
        for (int i = 0; i < tam; i++) {

            if (vertices[i] == rotuloVertice) {
                return i;
            }

        }
        return -1;

    }

    /**
    * O argumento indicesVerticesVisitados serve para controlar quais
    * vertices já foram visitados.
    * Lembrando que DFS é uma função recursiva.
    **/

    void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados) {
        //IMPLEMENTAR
        int index = obterIndiceVertice(rotuloVOrigem);

        if (indicesVerticesVisitados[index]) return;

        indicesVerticesVisitados[index] = true;

        for (auto& x : arestas[index]) {
            dfs(vertices.at(x.first), indicesVerticesVisitados);
        }

    }

public:
    /**
    * Lembrem-se:
    *       1) nao podemos inserir vertices com mesmo rotulo
    *       2) toda vez que inserirmos um novo vértice, precisaremos
    *          inserir um vetor para representar as conexões daquele
    *          vértice na lista de adjacências
    **/
    void inserirVertice(string rotuloVertice) {
        //IMPLEMENTAR
        int verify = obterIndiceVertice(rotuloVertice);

        if (verify == -1) {

            vector<pair<int, int>> temp;

            vertices.push_back(rotuloVertice);
            arestas.push_back(temp);

        }
    }

    /**
    * Sempre que o grafo for não ponderado, adicionaremos o peso 1,
    * por conveniência.
    **/
    void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
    }

    void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
        inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, 1);
    }

    void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, peso);
        inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, peso);
    }

    /**
    * Inserir uma aresta entre rotuloVOrigem e rotuloVDestino com o peso
    * especificado.
    **/
    void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
        //IMPLEMENTAR
        int fromIndex = obterIndiceVertice(rotuloVOrigem);
        int toIndex = obterIndiceVertice(rotuloVDestino);

        arestas.at(fromIndex).push_back(make_pair(toIndex, peso));
    }

    /**
    * Verifica se vértice rotuloVOrigem e vértice rotuloVDestino são
    * conectados (vizinhos).
    **/
    bool saoConectados(string rotuloVOrigem, string rotuloVDestino) {
        //IMPLEMENTAR
        int fromIndex = obterIndiceVertice(rotuloVOrigem);
        int toIndex = obterIndiceVertice(rotuloVDestino);

        for (int i = 0; i < arestas.at(fromIndex).size(); i++) {
            if (arestas.at(fromIndex).at(i).first == toIndex) {
                return true;
            }
        }

        return false;

    }

    /**
    * Verifica se há algum caminho entre vértice rotuloVOrigem e
    * vértice rotuloVDestino.
    * A melhor forma de fazer isto é reusando a função dfs.
    **/
    bool haCaminho(string rotuloVOrigem, string rotuloVDestino) {
        //IMPLEMENTAR
        int toIndex = obterIndiceVertice(rotuloVDestino);
        int fromIndex = obterIndiceVertice(rotuloVOrigem);

        size_t  tam = vertices.size();

        bool* visitados = new bool[tam];

        for (int i = 0; i < tam; i++) {
            visitados[i] = false;
        }

        if (toIndex == 0 && fromIndex == 0) {
            for (int i = 0; i < arestas[toIndex].size(); i++) {
                if (arestas[toIndex][i].first == toIndex) {
                    return true;
                }
            }
        }
        else {

            dfs(rotuloVOrigem, visitados);

        }
        bool verify = visitados[toIndex] ? true : false;
        delete[] visitados;
        return verify;

    }

    /**
    * Muda os rótulos do vértices dos diferentes componentes para
    * nomes que representam cores.
    * Dica: procura componentes partindo do vértice v0 ou v1, em ordem
    * crescente (mas você pode usar outra ordem se desejar).
    * Retorna a quantidade de componentes.
    * A melhor forma de fazer isto é reusando a função dfs.
    **/
    int colorir() {
        //IMPLEMENTAR
        int cor = 0;
        size_t  tam = vertices.size();

        bool* visitados = new bool[tam];

        for (int i = 0; i < tam; i++) {
            visitados[i] = false;
        }

        for (int i = 0; i < tam; i++) {
            if (!visitados[i]) {
                dfs(vertices[i], visitados);
                cor++;
            }

            for (int j = 0; j < tam; j++) {

                if (visitados[j] && vertices[j][0] == 'v') {

                    for (int k = 0; k < tam; k++) {
                        if (saoConectados(vertices[j], vertices[k])) {
                            if (vertices[k][0] == 'v') {
                                vertices[k] = to_string(cor);

                            }
                        }
                    }
                    vertices[j] = to_string(cor);
                }

            }
        }

        delete[] visitados;

        return cor;
    }

    /**
    * Usa a abordagem de navegação BFS para listar as distâncias
    * entre o vértice rotuloVOrigem e cada um dos demais vértices.
    * A função BFS consegue listar a menor distância entre um vértice
    * e os demais se o grafo for NÃO-PONDERADO.
    * Retorna um vetor de inteiros com a quantidade de arestas
    * (distância) e o vértice rotuloVOrigem e cada um dos demais vértices.
    * Não é uma função recursiva.
    * É necessário utilizar a ED fila.
    **/
    int* bfs(string rotuloVOrigem) {
        //IMPLEMENTAR

        int index = obterIndiceVertice(rotuloVOrigem);
        size_t  tam = vertices.size();
        queue<int> fila;
        bool* visitados = new bool[tam];
        int* distancia = new int[tam];

        for (int i = 0; i < tam; i++) {
            visitados[i] = false;
            distancia[i] = 0;
        }

        visitados[index] = true;
        fila.push(index);

        while (!fila.empty()) {
            index = fila.front();
            fila.pop();

            for (vector<pair<int, int>>::iterator i = arestas[index].begin(); i != arestas[index].end(); i++) {
                int temp = (*i).first;
                if (!visitados[temp]) {
                    visitados[temp] = true;
                    distancia[temp] = distancia[index] + 1;
                    fila.push(temp);
                }
            }
        }

        delete[] visitados;

        return distancia;

    }

    vector<string> getVertices() {
        return vertices;
    }

    vector<vector<pair<int, int>>> getArestas() {
        return arestas;
    }
};