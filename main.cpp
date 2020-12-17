// C++ code
#include <iostream>
#include <vector>
#include <iomanip>      // for setw()
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const int Max_Weight = INT16_MAX;    // 初始化key[]時需要infinity, 以Max_Weight代替

class GraphMST {

    private:
        int num_vertex;
        vector<vector<int>> AdjMatrix;
    public:
        GraphMST(): num_vertex(0) {};

        GraphMST(int n): num_vertex(n) {
            AdjMatrix.resize(num_vertex);
            for (int i = 0; i < num_vertex; i++) {
                AdjMatrix[i].resize(num_vertex);
            }
        }

        void AddEdge(int from, int to, int weight);

        void PrimMST(int Start = 0);        // 可以指定起點Start, 若沒有指定, 則從vertex(0)作為MST的root

        friend int MinKeyExtract(int *key, bool *visited, int size);
};

int MinKeyExtract(int *key, bool *visited, int size){

    int min = Max_Weight, min_idx = 0;
    for (int i = 0; i < size; i++) {
        if (visited[i]==false && key[i]<min) {
            min = key[i];
            min_idx = i;
        }
    }
    return min_idx;
}

void GraphMST::PrimMST(int Start) {
    int res = 0;
    int key[num_vertex], predecessor[num_vertex];
    bool visited[num_vertex];

    for (int i = 0; i < num_vertex; i++) {
        key[i] = Max_Weight;
        predecessor[i] = -1;
        visited[i] = false;     // false表示vertex還沒有被visited
    }

    key[Start] = 0;
    for (int i = 0; i < num_vertex; i++) {
        int u = MinKeyExtract(key, visited, num_vertex);
        visited[u] = true;
        for (int i = 0; i < num_vertex; i++) {
            if ( visited[i]==false && AdjMatrix[u][i]!=0 && AdjMatrix[u][i]<key[i]) {
                predecessor[i] = u;
                key[i] = AdjMatrix[u][i];
            }
        }
    }

    // print MST, 與MST演算法主體無關
    cout << setw(3) << "v1" << " - " << setw(3) << "v2"<< " : weight\n";

    int i = (Start + 1) % num_vertex;   // 若從4開始, i依序為5,6,0,1,2,3
    while (i != Start) {
        res += AdjMatrix[predecessor[i]][i];
        cout << setw(3) << predecessor[i] + 1 << " - " << setw(3) << i + 1
                  << " : " << setw(4) << AdjMatrix[predecessor[i]][i] <<"\n";
        i = (++i) % num_vertex;       // 到了6之後, 6+1 = 7, error:bad_access, 透過mod把7喬回0
    }
    cout << "Total cost = " << res << "\n========================\n" << endl;
}

void GraphMST::AddEdge(int from, int to, int weight){
    AdjMatrix[from][to] = weight;
}

vector<vector<int>> read_csv(string filePath, int n) {
    vector<vector<int>> data;
    ifstream csvFile(filePath);
    if (!csvFile.is_open()) throw runtime_error("Could not open file");
    string row, col;
    int cost = 0;

    for(auto i = 0; i < n; i++) {
        vector<int> tmp;
        getline(csvFile, row);
        stringstream ss(row);

        for(auto j = 0; j < n; j++) {
            getline(ss, col, ',');
            stringstream col_str(col);
            col_str >> cost;
            //cout<<cost<<" ";
            tmp.push_back(cost);
        }
        data.push_back(tmp);
    }
    return data;
}

int main(){

    for (auto k = 10; k <= 50; k += 10) {
        vector<vector<int>> data = read_csv("/Users/twlin/NCTU/Courses/Advanced_Algorithms/hw5/src/test.csv", k);
        GraphMST g(k);

        for (auto i = 0; i < data.size(); i++)
            for (auto j = 0; j < data[i].size(); j++)
                g.AddEdge(i, j, data[i][j]);

        cout << "when # of nodes are " << k << ", Edges of MST are \n";

        //start from 0
        g.PrimMST(0);
    }

    return 0;
}