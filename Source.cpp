#include <iostream>
#include <vector>
#include <time.h> 
#include <fstream>
#include <chrono>
#include <queue>
#include <utility>
#include <limits>
using namespace std;
using namespace std::chrono;
fstream out("results.txt", std::fstream::in | std::fstream::out | std::fstream::app);
vector<vector<int>> generate_random_adjacency_matrix(int num_vertices, int num_edges) {
	vector<vector<int>> adj(num_vertices, vector<int>(num_vertices, INT_MAX));
	srand(time(0));
	for (int i = 0; i < num_edges; ++i) {
		int node1 = rand() % num_vertices;
		int node2 = rand() % num_vertices;
		int weight = rand() % 10 + 1;
		if (node1 != node2 && adj[node1][node2] == INT_MAX && adj[node2][node1] == INT_MAX) {
			adj[node1][node2] = weight;
			adj[node2][node1] = weight;
		}
		else {
			--i;
		}
	}
	return adj;
}

void printSolution(vector<vector<int>> dist, int vertices)
{
	cout << "The following matrix shows the shortest distances between every pair of vertices: \n";
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < vertices; j++) {
			if (dist[i][j] == INT_MAX)
				cout << "0"
				<< " ";
			else
				cout << dist[i][j] << ", ";
		}
		cout << endl;
	}
}
void floydWarshall(vector<vector<int>>& dist, int vertices)
{
	int i, j, k;
	for (k = 0; k < vertices; k++) {
		for (i = 0; i < vertices; i++) {
			for (j = 0; j < vertices; j++) {
				if (dist[i][j] > (dist[i][k] + dist[k][j])
					&& (dist[k][j] != INT_MAX
						&& dist[i][k] != INT_MAX))
					dist[i][j] = dist[i][k] + dist[k][j];
			}
		}
	}
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < vertices; j++) {
			if (i == j) {
				dist[i][j] = 0;
			}
		}
	}
	//printSolution(dist, vertices);
}
int minDistance(vector<int> dist, vector<bool> sptSet, int vertices)
{
	int min = INT_MAX, min_index;
	for (int v = 0; v < vertices; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;
	return min_index;
}

void dijkstra(vector<vector<int>>& graph, int vertices)
{
	vector<vector<int>> dp(vertices, vector<int>(vertices, INT_MAX));
	for (int i = 0; i < vertices; i++) {
		dp[i][i] = 0; // distance to self is 0
		for (int j = 0; j < vertices; j++) {
			if (graph[i][j] != INT_MAX) {
				dp[i][j] = graph[i][j]; // direct edge
			}
		}
	}

	// compute shortest distances using dynamic programming
	for (int k = 0; k < vertices; k++) {
		for (int i = 0; i < vertices; i++) {
			for (int j = 0; j < vertices; j++) {
				if (dp[i][k] != INT_MAX && dp[k][j] != INT_MAX) {
					dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
				}
			}
		}
	}

	//printSolution(dp, vertices);
}

void outputMetrics(char f, char x,vector<vector<int>>graph, int vert, int i)
{
	string s;
	auto search_start = high_resolution_clock::now();
	switch (f)
	{
	case '1':
		dijkstra(graph, vert);
		s = " Djikstra: ";
		break;
	case '2':
		floydWarshall(graph, vert);
		s = " FloydWarshall: ";
		break;
	}
	auto search_stop = high_resolution_clock::now();
	auto duration_search = duration_cast<milliseconds>(search_stop - search_start);
	out << i << " " << x << s << "\t" << duration_search.count() / 1e3f << endl;
}

int main()
{
	for (int i = 750; i <= 1000; i += 50)
	{
		int vert = i;
		int edges = vert * (vert - 1) / 2;
		vector<vector<int>> graph1 = generate_random_adjacency_matrix(vert, edges); // first graph - dense
		vector<vector<int>> graph2 = graph1;
		//for (int i = 0; i < vert; ++i) {
		//	for (int j = 0; j < vert; ++j) {
		//		if (graph1[i][j] == INT_MAX)
		//		{
		//			cout << 0 << " ";
		//			continue;
		//		}
		//		cout << graph1[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
		//cout << endl;
		//vector<vector<int>> graph2 = graph1;
		vector<vector<int>> graph3 = generate_random_adjacency_matrix(vert, edges* 1/10 ); //second graph - sparse
		//for (int i = 0; i < vert; ++i) {
		//	for (int j = 0; j < vert; ++j) {
		//		if (graph3[i][j] == INT_MAX)
		//		{
		//			cout << 0 << " ";
		//			continue;
		//		}
		//		cout << graph3[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
		vector<vector<int>> graph4 = graph3;
		outputMetrics('1', 'd', graph1, vert, i);
		outputMetrics('2', 'd', graph2, vert, i);
		outputMetrics('1', 's', graph3, vert, i);
		outputMetrics('2', 's', graph4, vert, i);
	}
	
	return 0;
}