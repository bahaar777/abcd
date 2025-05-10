#include <omp.h>
#include <iostream>
#include <queue>
#include <stack>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

class Node {
public:
    int value;
    Node *left;
    Node *right;
    Node() {
        // Required empty constructor.
    }
    Node(int value) {
        this->value = value;
        this->left = NULL;
        this->right = NULL;
    }
};

Node *generateTree(int values[], int length) {
    if (length == 0)
        return NULL;
    Node *treeNodes[length];
    for (int i = 0; i < length; i++) {
        if (values[i] != -1)
            treeNodes[i] = new Node(values[i]);
        else
            treeNodes[i] = NULL;
    }
    int parent = 0;
    int child = 1;
    while (child < length) {
        if (treeNodes[parent] != NULL) {
            treeNodes[parent]->left = treeNodes[child++];
            if (child < length)
                treeNodes[parent]->right = treeNodes[child++];
        }
        parent++;
    }
    Node *root = treeNodes[0];
    return root;
}

void parallel_bfs(Node *root) {
    if (root == NULL)
        return;
    queue<Node *> q;
    q.push(root);

    while (!q.empty()) {
        int level_size = q.size();
        vector<Node *> current_level;

        // Extract current level
        for (int i = 0; i < level_size; i++) {
            Node *node = q.front();
            q.pop();
            cout << node->value << " -> ";
            current_level.push_back(node);
        }

        vector<Node *> next_level;

        // Parallelize over current level nodes
        #pragma omp parallel for
        for (int i = 0; i < current_level.size(); i++) {
            Node *node = current_level[i];
            #pragma omp critical
            {
                if (node->left != NULL)
                    next_level.push_back(node->left);
                if (node->right != NULL)
                    next_level.push_back(node->right);
            }
        }

        for (Node *node : next_level)
            q.push(node);
    }
}

void dfs(Node *root) {
    if (root == NULL)
        return;
    #pragma omp critical
    cout << root->value << " -> ";
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        dfs(root->left);
        #pragma omp section
        dfs(root->right);
    }
}

int main() {
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,34,32,45,67,89,42,66,23,43,87,21,45,67,88,99,64,30,65,76,87,98,92,93,94,95,96,97};
    int length = sizeof(values) / sizeof(int); // Calculate the length of array.
    Node *root = generateTree(values, length); // Generate the binary tree.

    // Measure execution time for BFS
    auto start_bfs = high_resolution_clock::now();
    cout << "BFS: ";
    parallel_bfs(root);
    auto stop_bfs = high_resolution_clock::now();
    auto duration_bfs = duration_cast<microseconds>(stop_bfs - start_bfs);
    cout << "\nExecution time for BFS: " << duration_bfs.count() << " microseconds" << endl;

    // Measure execution time for DFS
    auto start_dfs = high_resolution_clock::now();
    cout << "DFS: ";
    dfs(root);
    auto stop_dfs = high_resolution_clock::now();
    auto duration_dfs = duration_cast<microseconds>(stop_dfs - start_dfs);
    cout << "\nExecution time for DFS: " << duration_dfs.count() << " microseconds" << endl;

    return 0;
}
