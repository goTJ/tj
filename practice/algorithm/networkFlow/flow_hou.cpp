
// 使用多輸入

//////////////////////////
#include <iostream>
#include <cstdlib>

using namespace std;
ostream& out = cout;
istream& in = cin;

#undef DEBUG

//////////////////////////

unsigned int N, M;
unsigned int c[300][300];
unsigned int S, T;

void init () {
	for (unsigned int i = 0; i < N; i++)
		for (unsigned int j = 0; j < N; j++)
			c[i][j] = 0;
}

void add (int i, int j, int __c) {
	c[i][j] += __c;
 	c[j][i] += __c;
}

bool marked[300];
void init_dfs () {
	for (unsigned int i = 0; i < N; i++)
		marked[i] = false;
}
bool dfs (unsigned int i, unsigned int bound) {
	if (i == T) return true;
	marked[i] = true;

	for (unsigned int j = 0; j < N; j++){
		if (marked[j]) continue;
		if (c[i][j] < bound) continue;
		if (dfs(j, bound)) {
			c[i][j] -= bound;
			c[j][i] += bound;
			
			marked[i] = false;
			return true;
		}
	}
	return false;
}

bool dfs (unsigned int bound) {
	init_dfs();

	return dfs (S, bound);
}

int get_max () {
	unsigned int max = 0;

	// 奧義：二分逼近 
	unsigned int bound = 0;
	for (unsigned int i = 0; i < N; i++)
		for (unsigned int j = 0; j < N; j++)
			if (c[i][j] > bound)
				bound = c[i][j];
	while (bound > 0) {
		while (dfs(bound)) {
			max+=bound;
		}
		bound/=2;
	}
	return max;
}

int main () {
	try {
		if (in) {
			in >> N;
			if (in.fail() || N > 300) throw "Nothing";

			in >> M;
			if (in.fail() || M > 1000) throw "Nothing";
		
			init ();
			for (unsigned int i = 0; i < M; i++) {
				unsigned int u, v, w;
				in >> u >> v >> w;
				
				if (in.fail() || u == 0 || u > N || v == 0 || v > N || w > 100)
					throw "Nothing";
#ifdef DEBUG
				out << u << ' ' << v << ' ' << w << endl;
#endif
				add (u-1, v-1, w);
			}
			in >> S >> T;
			if (in.fail() || S == 0 || S > N || T == 0 || T > N)
				throw "Nothing";

			S--, T--;
		
			if (S==T) {
				out << "∞" << endl;
				return 0;
			}
		
#ifdef DEBUG
			out << S << ' ' << T << endl;
#endif
			out << get_max () << endl;
		}
	}
 	catch (...) {
		out << endl;
		out << "----------------------------------------------------------------" << endl;
		out << "This is a program computing the max-flow of a undirected graph." << endl;
		out << endl;
		out << "----------------------------------------------------------------" << endl;
		out << "Input:" << endl;
		out << "Line 001: N ==> represent the number of vertexes" << endl;
		out << "Line 002: M ==> represent the number of edges" << endl;
		out << "Line 003: u1, v1, w1 ==> the capacity of edge(ui, vi) is wi" << endl;
		out << "Line 004: u2, v2, w2" << endl;
		out << "      .   .   .   ." << endl;
		out << "      .   .   .   ." << endl;
		out << "      .   .   .   ." << endl;
		out << "Line M+2: uM, vM, wM" << endl;
		out << "Line M+3: S ==> represent the Sink" << endl;
		out << "Line M+4: T ==> represent the Tank" << endl;
		out << endl;
		out << "Note: 1 <= u,v <= N <= 300, 1 <= M <= 1000, 1 <= w <= 100" << endl;
		out << endl;
		out << "----------------------------------------------------------------" << endl;
		out << "Output:" << endl;
		out << "Line 001: max ==> the value of max-flow" << endl;
		out << endl;
		out << "----------------------------------------------------------------" << endl;
		system("PAUSE");
	}
	return 0;
}
