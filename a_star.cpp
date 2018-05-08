#include <bits/stdc++.h>

using namespace std;


const int dc[4] = {0,0,1,-1}; //dir colums
const int dr[4] = {1,-1,0,0}; // dir rows
const int pixels = 482;
const int expect_row = 479;
const int expect_col = 239;
const int start_row_global = 3;
const int start_col_global = 239;



int H(int row, int col) {
	
	int h = abs(row - expect_row) + abs(col - expect_col);
	return h;
}


class Node {
private:
	int row, col, g, h;

public:	
	Node() {g=0; h = H(row, col);}
	Node(int x, int y, int g_pass) {
		this->row = x, this->col = y;
		g=g_pass;
		h = H(this->row, this->col);
	}

	bool operator < ( const Node &curr) const{
		return (g + h) > (curr.g + curr.h);
	}
	int get_G() { return this->g; }
	int get_row()  { return this->row; }
	int get_col()  { return this->col; }
	
};



	

bool A_star(int start_row, int start_col, int **vis, int **graph) {
	
	Node start(start_row, start_col, 0);
	
	priority_queue<Node> PQ;
	
	PQ.push(start);
	
	while(!PQ.empty()) {
		Node curr = PQ.top();
		
		if(curr.get_row() == expect_row and curr.get_col() == expect_col) {
			return true;
		}
		PQ.pop();
		
		for(int i = 0; i < 4; ++i) {
			int n_row = curr.get_row() + dr[i];
			int n_col = curr.get_col() + dc[i];
			
			if(n_row < 0 or n_row >= pixels) continue;
			if(n_col < 0 or n_col >= pixels) continue;

			Node node(n_row, n_col, curr.get_G() + 1);
			
			if(!vis[node.get_row()][node.get_col()] and !graph[node.get_row()][node.get_col()]) {
				PQ.push(node);
				vis[node.get_row()][node.get_col()] = vis[curr.get_row()][curr.get_col()] + 1;
			}
		}
	}
	return false;
}
	
	
vector<pair<int, int> > reverse_way(int **vis) {
	
	vector<pair<int, int> > res;	
	
	int value = vis[expect_row][expect_col];

	int ni = expect_row, nj = expect_col;

	while(ni != start_row_global || nj != start_col_global) {
		if(value < 5) break;

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

	vector<pair<int, int> > path;
	
	if(!A_star(start_row_global, start_col_global, vis, graph)) fprintf(stderr, "ERROR\n");
	else path = reverse_way(vis);
	
	
	printf("%lu\n", path.size());
	
	for(int i = path.size()-1; i >= 0; --i)
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
	

