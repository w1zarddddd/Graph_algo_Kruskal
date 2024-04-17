#include <iostream>
#include <locale.h>
struct Edge {
    int nodeSTART;
    int nodeEND;
    int wght;
    Edge* next;

    Edge() : nodeSTART(0), nodeEND(0), wght(0), next(nullptr) {}
    Edge(int node_1, int node_2, int weight) : nodeSTART(node_1), nodeEND(node_2), wght(weight), next(nullptr) {}
};

class G {
private:
    int num_of_nodes;
    Edge* head;
    int edgeCount;

public:
    G() : num_of_nodes(0), head(nullptr), edgeCount(0) {}
    void AddEdgeGraph(int node_1, int node_2, int weight);
    void RemoveEdgeGraph(int node_1, int node_2);
    bool IsEdgeExist(int node_1, int node_2);
    void KruskalMST_ALGO(Edge* result);
    int FindparentNode(int node, int* parentNode);
    void bubbleSort(Edge*& head);
    void DisplayEdgeGraphs(Edge* mst, int size, int totalWeight);
    bool IsUniqueMST(Edge* mst, int size);
    int CountNodes();
    bool IsMinimumSpanningTree(Edge* edges, int size, int totalWeight);

    ~G() {
        Edge* current = head;
        while (current != nullptr) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

int G::CountNodes() {
    if (!head) {
        return 0;
    }

    int count = 0;
    bool* visited = new bool[num_of_nodes + 1];
    for (int i = 0; i <= num_of_nodes; ++i) {
        visited[i] = false;
    }

    Edge* current = head;
    while (current != nullptr) {
        if (!visited[current->nodeSTART]) {
            visited[current->nodeSTART] = true;
            count++;
        }
        if (!visited[current->nodeEND]) {
            visited[current->nodeEND] = true;
            count++;
        }
        current = current->next;
    }

    delete[] visited;
    return count;
}

void G::AddEdgeGraph(int node_1, int node_2, int weight) {
    if (!IsEdgeExist(node_1, node_2)) {
        Edge* newEdge = new Edge(node_1, node_2, weight);
        newEdge->next = head;
        head = newEdge;
        ++edgeCount;
        if (node_1 > num_of_nodes) num_of_nodes = node_1;
        if (node_2 > num_of_nodes) num_of_nodes = node_2;
    }
    else {
        std::cout << "Edge [" << node_1 << "-" << node_2 << "] already exists." << std::endl;
    }
}

void G::RemoveEdgeGraph(int node_1, int node_2) {
    Edge* prev = nullptr;
    Edge* current = head;
    while (current != nullptr) {
        if ((current->nodeSTART == node_1 && current->nodeEND == node_2) ||
            (current->nodeSTART == node_2 && current->nodeEND == node_1)) {
            if (prev == nullptr) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            --edgeCount;
            return;
        }
        prev = current;
        current = current->next;
    }
    std::cout << "Edge [" << node_1 << "-" << node_2 << "] not found." << std::endl;
}

bool G::IsEdgeExist(int node_1, int node_2) {
    Edge* current = head;
    while (current != nullptr) {
        if ((current->nodeSTART == node_1 && current->nodeEND == node_2) ||
            (current->nodeSTART == node_2 && current->nodeEND == node_1)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int G::FindparentNode(int node, int* parentNode) {
    if (node != parentNode[node])
        parentNode[node] = FindparentNode(parentNode[node], parentNode);
    return parentNode[node];
}

void G::bubbleSort(Edge*& head) {
    if (!head || !head->next)
        return;

    Edge* temp;
    bool swapped;
    do {
        swapped = false;
        temp = head;
        while (temp->next != nullptr) {
            if (temp->wght > temp->next->wght ||
                (temp->wght == temp->next->wght && temp->nodeSTART > temp->next->nodeSTART) ||
                (temp->wght == temp->next->wght && temp->nodeSTART == temp->next->nodeSTART && temp->nodeEND > temp->next->nodeEND)) {
                int tempSTART = temp->nodeSTART;
                int tempEND = temp->nodeEND;
                int tempWeight = temp->wght;

                temp->nodeSTART = temp->next->nodeSTART;
                temp->nodeEND = temp->next->nodeEND;
                temp->wght = temp->next->wght;

                temp->next->nodeSTART = tempSTART;
                temp->next->nodeEND = tempEND;
                temp->next->wght = tempWeight;

                swapped = true;
            }
            temp = temp->next;
        }
    } while (swapped);
}

bool G::IsMinimumSpanningTree(Edge* edges, int size, int totalWeight) {
    // Check if the current set of edges is a minimum spanning tree
    if (size != num_of_nodes - 1) {
        return false;
    }

    // Check if all edge weights are equal
    for (int i = 1; i < size; ++i) {
        if (edges[i].wght != edges[0].wght) {
            return false;
        }
    }

    // Edge weights are equal
    return true;
}

void G::KruskalMST_ALGO(Edge* result) {
    // Find the first minimum spanning tree
    Edge* sortedEdges = nullptr;

    Edge* current = head;
    while (current != nullptr) {
        Edge* newEdge = new Edge(current->nodeSTART, current->nodeEND, current->wght);
        newEdge->next = sortedEdges;
        sortedEdges = newEdge;
        current = current->next;
    }

    bubbleSort(sortedEdges);

    int* parentNode = new int[num_of_nodes + 1];
    for (int i = 0; i <= num_of_nodes; ++i)
        parentNode[i] = i;

    int edgeTaken = 0;
    int mstWeight = 0;
    while (sortedEdges != nullptr) {
        if (edgeTaken == num_of_nodes - 1) {
            break;
        }

        int root_1 = FindparentNode(sortedEdges->nodeSTART, parentNode);
        int root_2 = FindparentNode(sortedEdges->nodeEND, parentNode);

        if (root_1 != root_2) {
            result[edgeTaken++] = *sortedEdges;
            mstWeight += sortedEdges->wght;
            if (root_1 < root_2) {
                parentNode[root_2] = root_1;
            }
            else {
                parentNode[root_1] = root_2;
            }
        }

        Edge* temp = sortedEdges;
        sortedEdges = sortedEdges->next;
        delete temp;
    }

    delete[] parentNode;

    // Check if there is another minimum spanning tree with the same weight
    bool hasOtherMST = false;
    current = head;
    while (current != nullptr) {
        Edge* tempResult = new Edge[num_of_nodes - 1];
        int tempEdgeTaken = 0;
        int tempMSTWeight = 0;

        int* tempParentNode = new int[num_of_nodes + 1];
        for (int i = 0; i <= num_of_nodes; ++i)
            tempParentNode[i] = i;

        Edge* tempCurrent = head;
        while (tempCurrent != nullptr) {
            if (tempCurrent != current) {
                int root_1 = FindparentNode(tempCurrent->nodeSTART, tempParentNode);
                int root_2 = FindparentNode(tempCurrent->nodeEND, tempParentNode);

                if (root_1 != root_2) {
                    tempResult[tempEdgeTaken++] = *tempCurrent;
                    tempMSTWeight += tempCurrent->wght;
                    if (root_1 < root_2) {
                        tempParentNode[root_2] = root_1;
                    }
                    else {
                        tempParentNode[root_1] = root_2;
                    }
                }
            }

            tempCurrent = tempCurrent->next;
        }

        if (IsMinimumSpanningTree(tempResult, tempEdgeTaken, tempMSTWeight)) {
            hasOtherMST = true;
            delete[] tempResult;
            delete[] tempParentNode;
            break;
        }

        delete[] tempResult;
        delete[] tempParentNode;

        current = current->next;
    }

    if (hasOtherMST) {
        std::cout << "Минимальное остовное дерево не единственное." << std::endl;
    }
    else {
        std::cout << "Минимальное остовное дерево единственное." << std::endl;
    }

    DisplayEdgeGraphs(result, edgeTaken, mstWeight);
}

void G::DisplayEdgeGraphs(Edge* mst, int size, int totalWeight) {
    std::cout << "Edges: ";
    for (int i = 0; i < size; ++i) {
        std::cout << "[" << mst[i].nodeSTART << "-" << mst[i].nodeEND
            << "](" << mst[i].wght << ") ";
    }
    std::cout << "\nTotal Weight: " << totalWeight << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    G g1;

    g1.AddEdgeGraph(1, 2, 3);
    g1.AddEdgeGraph(1, 6, 3);
    g1.AddEdgeGraph(2, 3, 5);
    g1.AddEdgeGraph(2, 5, 4);
    g1.AddEdgeGraph(2, 7, 3);
    g1.AddEdgeGraph(3, 5, 2);
    g1.AddEdgeGraph(3, 8, 4);
    g1.AddEdgeGraph(4, 5, 2);
    g1.AddEdgeGraph(4, 6, 3);
    g1.AddEdgeGraph(4, 7, 2);
    g1.AddEdgeGraph(5, 8, 3);
    g1.AddEdgeGraph(6, 7, 1);
    g1.AddEdgeGraph(7, 8, 1);

    int num_of_nodes = g1.CountNodes();
    std::cout << "Количество вершин в графе: " << num_of_nodes << std::endl;

    Edge* mst = new Edge[num_of_nodes - 1];
    g1.KruskalMST_ALGO(mst);

    delete[] mst;

    return 0;
}
