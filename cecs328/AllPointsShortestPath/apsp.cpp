#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <string>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;
//struct params {
//    int n;
//    int i;
//    vector<vector<int>>* dist;
//    params(int n_i, int i_i, vector<vector<int>> &dist_i){
//        n = n_i;
//        i = i_i;
//        dist = &dist_i;
//    }
//};
//
//void do_for_i(params pass) {
//    int n = pass.n;
//    int i = pass.i;
//    vector<vector<int>> dist = *(pass.dist);
//
//    for (int j = 0; j < n; j++) {
//        for (int k = 0; k < n; k++) {
//            if (i == j) {
//                continue;
//            }
//            if ((dist[i][j] > dist[i][k] + dist[k][j]) && dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
//                dist[i][j] =  dist[i][k] + dist[k][j];
//            }
//        }
//    }
//
//    cout << i << " thread done. " << endl;
//}

vector<vector<int>> shortest_path(vector<vector<int>> &V) {
    int largest = 0;
    for (int i = 0; i < V.size(); i++) {
        if (V[i][0] > largest) {
            largest = V[i][0];
        }
        else if (V[i][1] > largest) {
            largest = V[i][1];
        }
    }

    vector<vector<int>> adj_mat;
    for (int i = 0; i < largest; i++){
        vector<int> temp(largest, INT_MAX);
        adj_mat.push_back(temp);
    }

    for (int i = 0; i < V.size(); i++) {
        adj_mat[V[i][0] - 1][V[i][1] - 1] = V[i][2];
        adj_mat[V[i][1] - 1][V[i][0] - 1] = V[i][2];
    }


    int n = adj_mat.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                adj_mat[i][j] = 0;
                adj_mat[j][i] = 0;
            }
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adj_mat[i][k] != INT_MAX && adj_mat[k][j] != INT_MAX) {
                    int p = min(adj_mat[i][j], adj_mat[i][k] + adj_mat[k][j]);
                    adj_mat[i][j] = p;
                    adj_mat[j][i] = p;
                }
                /*if(pre!=post){
                    cout << "Pre: " << pre << endl;
                    cout << "L add: " << dist[i][k] << endl;
                    cout << "R add: " << dist[k][j] << endl;
                    cout << "ADD= " << dist[i][k] + dist[k][j] << endl;
                    cout << "(" << i << ", " << j << ") = " << dist[i][j] << endl;
                }*/
                
            }
        }
    }

    /*vector<params> passes;
    vector<thread> threads;
    for (int i = 0; i < n; i++){
        passes.emplace_back(n, i, adj_mat);
    }
    for (int l = 0; l < n; l++) {
        threads.emplace_back(do_for_i, passes[l]);
    }

    for (auto& th : threads) {
        th.join();
    }*/
    return adj_mat;
}

void process_dist() {
    vector<vector<int>> V;

    ifstream file("graph.txt");
    string file_str;
    string line;
    while (getline(file, line)) {
        file_str += line;
    }
    file.close();

    string buffer;
    for (int i = 0; i < file_str.length(); i++) {
        if (file_str[i] != '\0') {
            if ((file_str[i] == ',' || file_str[i] == '}') && file_str[i - 1] == '}') {
                vector<int> temp;
                string listBuffer;
                char removeThese[] = " {}";
                for (char c : removeThese) {
                    for (int i = 0; i < buffer.size(); i++) {
                        if (buffer[i] == c) {
                            buffer.erase(buffer.begin() + i);
                            i--;
                        }
                    }
                }
                for (int i = 0; i < buffer.size(); i++) {
                    if (buffer[i] == ',') {
                        temp.push_back(atoi(listBuffer.c_str()));
                        listBuffer = "";
                    }
                    else {
                        listBuffer += buffer[i];
                    }
                }
                temp.push_back(atoi(listBuffer.c_str()));
                buffer = "";
                V.push_back(temp);
            }
            else {
                buffer += file_str[i];
            }
        }
    }



    vector<vector<int>> adj_mat = shortest_path(V);
    int n = adj_mat.size();

    string distOutput = "{";
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            distOutput += ',';
        }
        distOutput += '{';
        for (int j = 0; j < n - 1; j++) {
            distOutput += to_string(adj_mat[i][j]);
            distOutput += ',';
        }
        distOutput += to_string(adj_mat[i][n - 1]);
        distOutput += '}';
    }
    distOutput += '}';

    ofstream output("dist.out");
    output << distOutput;
    output.close();
}

vector<vector<int>> process_dist_out() {
    ifstream dist_file("dist.out");
    string line;
    string dist_str;
    while (getline(dist_file, line)) {
        dist_str += line;
    }
    dist_file.close();

    vector<vector<int>> dist;

    string buffer;
    for (int i = 0; i < dist_str.length(); i++) {
        if (dist_str[i] != '\0') {
            if ((dist_str[i] == ',' || dist_str[i] == '}') && dist_str[i - 1] == '}') {
                vector<int> temp;
                string listBuffer;
                char removeThese[] = " {}";
                for (char c : removeThese) {
                    for (int i = 0; i < buffer.size(); i++) {
                        if (buffer[i] == c) {
                            buffer.erase(buffer.begin() + i);
                            i--;
                        }
                    }
                }
                for (int i = 0; i < buffer.size(); i++) {
                    if (buffer[i] == ',') {
                        temp.push_back(atoi(listBuffer.c_str()));
                        listBuffer = "";
                    }
                    else if (buffer[i] == '}') {
                        temp.push_back(atoi(listBuffer.c_str()));
                        listBuffer = "";
                    }
                    else if (i == buffer.size()-1) {
                        listBuffer += buffer[i];
                        temp.push_back(atoi(listBuffer.c_str()));
                        listBuffer = "";
                    }
                    else {
                        listBuffer += buffer[i];
                    }
                }
                buffer = "";
                dist.push_back(temp);
            }
            else {
                buffer += dist_str[i];
            }
        }
    }

    return dist;
}

int main() {

    if (!(ifstream("dist.out"))) {
        process_dist();
    }
    vector<vector<int>> dist = process_dist_out();
    cout << "RESULTS" << endl;

    cout << "#0: " << dist[0][4] << endl
        << "#1: " << dist[969][373] << endl
        << "#2: " << dist[95][115] << endl
        << "#3 : " << dist[792][941] << endl
        << "#4 : " << dist[89][102] << endl
        << "#5 : " << dist[369][315] << endl
        << "#6 : " << dist[641][7] << endl
        << "#7 : " << dist[68][373] << endl
        << "#8 : " << dist[252][726] << endl
        << "#9 : " << dist[373][115] << endl
        << "#10 : " << dist[264][508] << endl;

}