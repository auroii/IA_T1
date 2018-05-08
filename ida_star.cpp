#include <bits/stdc++.h>

using namespace std;


const int dc[4] = {0,0,1,-1}; //dir colums
const int dr[4] = {1,-1,0,0}; // dir rows
const int pixels = 482;
const int FOUND = -1;
const int INF = 0x3f3f3f3f;
const int expect_row = 479;
const int expect_col = 239;
const int start_row_global = 3;
const int start_col_global = 239;



//heuristica, distancia manhatan
int H(int row, int col) {
	int drow = expect_row - row;
	int dcol = expect_col - col;

	return abs(drow) +  abs(dcol);
}
	


int dfs(int g, int threshold, int **graph, vector<pair<int, int> > &path) {
	pair<int, int> node = path[path.size()-1];
	
		
	
	int h = H(node.first, node.second);

	int f = g + h;

	//fprintf(stderr, "%d\n%d %d\n", h, node.first, node.second);

	if(f > threshold) return f;

	if(h == 0) return FOUND;

	int minimal = INF;

	for(int i = 0; i < 4; ++i) {
		
		int new_row = node.first + dr[i];
		int new_col = node.second + dc[i];
		
		if(new_row < 0 or new_row >= pixels) continue;
		if(new_col < 0 or new_col >= pixels) continue;
		if(graph[new_row][new_col])  continue;
		
			
		path.push_back(make_pair(new_row, new_col));
		
		graph[new_row][new_col] = 2;

		int t = dfs(g+1, threshold, graph, path);
		
		if(t == FOUND) return FOUND;
		if(t < minimal) minimal = t;
		
		graph[new_row][new_col] = 0;

		path.pop_back();	
		
	}
	
	return minimal;
}
		
				
	

bool IDAstar(int start_row, int start_col, int **graph, int **vis, vector<pair<int, int> > &path) {
	int threshold = H(start_row, start_col);

	path.push_back(make_pair(start_row, start_col));
	
	while(1 < 2) {
		int t = dfs(0, threshold, graph, path);
		if(t == FOUND) return true;
		if(t >= INF) return false;
		
		
		threshold = t;
	}	
				
	return false;
}
		
	
enum {
	PROGNAME, FILENAME
};


int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[FILENAME], "r");
	
	
	vector<pair<int, int> > path;
	
	int **graph = new int*[pixels];
	for(int i = 0; i < pixels; ++i) {
		graph[i] = new int[pixels];
	}

	int **vis = new int*[pixels];
	
	for(int i = 0; i < pixels; ++i) {
		vis[i] = new int[pixels];
	}

	for(int i = 0; i < pixels; ++i) memset(vis[i], 0, sizeof(int) * pixels);
	
	for(int i = 0; i < pixels; ++i) {
		for(int j = 0; j < pixels; ++j) {
			fscanf(fp, "%d", &graph[i][j]);
		}
	}
	
	
	if(!IDAstar(start_row_global, start_col_global, graph, vis, path)) fprintf(stderr, "ERROR\n");
	
	
	printf("%lu\n", path.size());
	
	for(int i = 0; i < path.size(); ++i)
		printf("%d %d\n", path[i].first, path[i].second);
	


	for(int i = 0; i < pixels; ++i) {	
		delete [] graph[i];
	}
	
	delete [] graph;
	
	for(int i = 0; i < pixels; ++i) {	
		delete [] vis[i];
	}
	
	delete [] vis;
	
	fclose(fp);
	
	return 0;
}	
	

