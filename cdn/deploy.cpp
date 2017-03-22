#include "deploy.h"
#include <stdio.h>

struct Edge{
	int from,to,cap,flow,cost;
	Edge(){};
	Edge(int u,int v,int c,int f,int w):from(u),to(v),cap(c),flow(f),cost(w){}
};

struct MCMF{
	static const INF = 100010;
	static const FLOW_INF = 5010;

	int n,m;
	vector<Edge> edges;
	vector<int> G[maxn]; /*need declear maxn*/
	int inq[maxn];
	int d[maxn];
	int p[maxn]; /*last edge?*/
	int a[maxn];

	vector<int> end;
	vector<int> end_from;
	vector<int> need;

	void init(int n){
		this->n = n;
		for(int i=0;i<n;i++) G[i].clear();

		edges.clear();

		end.clear();
		need.clear();
		end_from.clear();
	}

	void AddEdge(int from,int to,int cap,int cost){
		edges.push_back(Edge(from,to,cap,0,cost));
		edges.push_back(Edge(to,from,0,0,-cost));
		m = edges.size();
		G[from].push_back(m-2);
		G[to].push_back(m-1);

		
	}

	/*
	void DeleteEdge(int times)
	{
		while(times>0)
		{
			times--;
			int from = edges.back().from;
			int to = edges.back().to;
			G[from].pop_back();
			G[to].pop_back();

			edges.pop_back();
			edges.pop_back();
			m=edges.size();
		}
	}
	*/

	bool BellmanFord(int s,int t,int &flow,long long &cost){
		for(int i=0;i<n;i++) d[i] = INF;
			memset(inq,0,sizeof(inq));
		d[s]=0;inq[s]=1;p[s]=0;a[s]=INF;

		queue<int> Q;
		Q.push(s);

		while(!Q.empty())
		{
			int u = Q.front();Q.pop();
			inq[u]=0;
			for(int i=0;i<G[u].size();i++)
			{
				Edge &e = edges[ G[u][i] ];
				if( e.cap > e.flow && d[e.to] > d[u] + e.cost ){
					d[e.to] = d[u] + e.cost;
					p[e.to] = G[u][i];
					a[e.to] = min(a[u], e.cap - e.flow);

					if(!inq[e.to]){
						Q.push(e.to);
						inq[e.to] = 1;
					}
				}
			}
		}

		if( d[t] == INF) return false;
		flow += a[t];
		cost += (long long) d[t] * (long long)a[t];
		for(int u=t;u!=s; u=edges [ p[u] ].from ){
			edges[ p[u] ].flow += a[t];
			edges[ p[u]^1 ].flow -= a[t];
		}
		return true;

	}

	int MincostMaxflow(int s,int t,long long &cost){
		int flow = 0;
		cost = 0;

		while(BellmanFord(s,t,flow,cost));
		return flow;

	}

	void build_graph()
	{
		for(int i=0;i<end_info;i++)
		{
			AddEdge(end[i], n-1 , need[i], 0 );
		}
	}

	/*need inplement*/

	void get_ans( const std::vector<int> &start)
	{
		init(number_of_node+1);//we assume that n+1 is the real end
		int assume_end = number_of_node;
		build_graph();
	}

}worker;

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{

	// 需要输出的内容
	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}
