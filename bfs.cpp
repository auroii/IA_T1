#include <bits/stdc++.h>

using namespace std;


const int dc[4] = {0,0,1,-1}; //dir colums
const int dr[4] = {1,-1,0,0}; // dir rows
const int pixels = 482;
const int expect_row = 479;
const int expect_col = 239;
const int start_row_global = 3;
const int start_col_global = 239;

int graph[pixels][pixels];
int vis[pixels][pixels];

	

bool BFS(int start_row, int start_col) {
	
	pair<int, int> start = make_pair(start_row, start_col);
	
	queue<pair<int, int> > q;
	
	q.push(start);

	while(!q.empty()) {
		pair<int, int> node = q.front();
		
		if(node.first == expect_row and node.second == expect_col) {
			return true;
		}
		q.pop();
		
		for(int i = 0; i < 4; ++i) {
			int n_row = node.first + dr[i];
			int n_col = node.second + dc[i];
			
			if(n_row < 0 or n_row >= pixels) continue;
			if(n_col < 0 or n_col >= pixels) continue;

			pair<int, int> current = make_pair(n_row, n_col);
			
			if(!vis[current.first][current.second] and !graph[current.first][current.second]) {
				q.push(current);
				vis[current.first][current.second] = vis[node.first][node.second] + 1;
			}
		}
	}
	return false;
}
	
	
vector<pair<int, int> > reverse_way(void) {
	
	vector<pair<int, int> > res;	
	
	int value = vis[expect_row][expect_col];

	int ni = expect_row, nj = expect_col;

	while(ni != start_row_global || nj != start_col_global) {
		if(value < 5) break;
		//fprintf(stderr, "%d\n", value);
		for(int i = 0; i < 4; ++i) {
			int x = ni + dr[i];
			int y = nj + dc[i];

			if(vis[x][y] == value-1) {
				res.push_back(make_pair(x, y));
				value--;
				ni = x;
				nj = y;
				break;
			}
		}
	}

	return res;	
}
			

	
	
enum {
	PROGNAME, FILENAME
};


int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[FILENAME], "r");
	
	
	for(int i = 0; i < pixels; ++i) {
		for(int j = 0; j < pixels; ++j) {
			fscanf(fp, "%d", &graph[i][j]);
		}
	}

	vector<pair<int, int> > path;
	
	if(!BFS(start_row_global, start_col_global)) fprintf(stderr, "ERROR\n");
	else path = reverse_way();
	
	
	printf("%lu\n", path.size());
	
	for(int i = path.size()-1; i >= 0; --i)
		printf("%d %d\n", path[i].first, path[i].second);
	
	
	fclose(fp);

	return 0;
}	
	

