#include "deploy.h"
/*#include <stdio.h>*/

#include <bits/stdc++.h>
using namespace std;


struct Edge{
	int from,to,cap,flow,cost;
	Edge(){};
	Edge(int u,int v,int c,int f,int w):from(u),to(v),cap(c),flow(f),cost(w){}
};

struct MCMF{
	static const INF = 100010;
	static const FLOW_INF = 5010;
	static const int maxn = 1002;

	int n,m;
	vector<Edge> edges;
	vector<int> G[maxn]; /*need declear maxn*/
	int inq[maxn];
	int d[maxn];
	int p[maxn]; /*last edge?*/
	int a[maxn];

	

	/*链路信息*/


	void init(int n){
		this->n = n;
		for(int i=0;i<n;i++) G[i].clear();

		edges.clear();
	}

	void AddEdge(int from,int to,int cap,int cost){
		edges.push_back(Edge(from,to,cap,0,cost));
		edges.push_back(Edge(to,from,0,0,-cost));
		m = edges.size();
		G[from].push_back(m-2);
		G[to].push_back(m-1);

		
	}

	
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
	

	bool BellmanFord(int s,int t,int &flow,int &cost){
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
		cost +=  d[t] * a[t];
		for(int u=t;u!=s; u=edges [ p[u] ].from ){
			edges[ p[u] ].flow += a[t];
			edges[ p[u]^1 ].flow -= a[t];
		}
		return true;

	}

	int MincostMaxflow(int s,int t,int &cost){
		int flow = 0;
		cost = 0;

		while(BellmanFord(s,t,flow,cost));
		return flow;

	}

	/*消费者信息*/

	/*
	vector<int> end;
	vector<int> end_from;
	vector<int> need;
	*/

	/*链路信息*/
	/*
	vector<int> path_start;
	vector<int> path_end;
	vector<int> path_flow;
	vector<int> path_cost;
	*/

	void build_graph(
		const vector<int> &end, const vector<int> &end_from,const vector<int> &need,
		const vector<int> &path_start, const vector<int> &path_end, const vector<int> &path_flow, const vector<int> &path_cost,
		int number_of_node
		)
	{
		init(number_of_node+1);

		for(int i=0;i<end.size();i++)
		{
			AddEdge(end[i], number_of_node , need[i], 0 );
		}
		for(int i=0;i<end.size();i++)
		{
			AddEdge(end_from[i],end[i],need[i],0);
		}

		for(int i=0;i<path_start.size();i++)
		{
			AddEdge(path_start[i],path_end[i],path_flow[i],path_cost[i]);
			AddEdge(path_end[i],path_start[i],path_flow[i],path_cost[i]);
		}


	}

	void make_start(const vector<int> &start)
	{
		for(int i=1;i<start.size();i++)
		{
			AddEdge(start[0],start[i],INF,0);
		}
	}

	/*-1 for no solution*/
	int get_cost( const vector<int> &start,
		//int number_of_node,int tot_need
		)
	{
		//init(number_of_node+1);//we assume that n+1 is the real end
		//int assume_end = number_of_node;
		//build_graph();

		if(start.size()==0)
		{
			cout << "empty start" << endl;
			return -1;
		}

		make_start();
		int cost;
		int flow = MincostMaxflow(start[0],number_of_node+1,cost);
		
		DeleteEdge(start.size()-1);
		
		if(flow < tot_need) return -1;
		return cost;		

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

void change(vector<int> &start,bool center[])
{
	srand(time(NULL)) ;
	int tmp = rand()%3 ;
	if(tmp == 0)
	{
		int pos = rand()%n ;
		while(center[pos]) pos = rand()%n ;
		center[pos] = 1 ;
		start.append(pos) ;
	}
	else if(tmp == 1)
	{
		int pos = rand()%(start.size()) ;
		int ele = start[pos] ;
		start.erase(pos) ;
		center[ele] = 0 ;
	}
	else
	{
		int pos2 = rand()%(start.size()) ;
		int ele = start[pos2] ;
		start.erase(pos2) ;
		center[ele] = 0 ;
		
		int pos1 = rand()%n ;
		while(center[pos1]) pos1 = rand()%n ;
		center[pos1] = 1 ;
		start.append(pos1) ;
	}
	
}	
`
void SA()
{
	bool center[n+1] = {0} ;
	vector<int> start ;
	for(int i = 0 ; i < n+1 ; i++)
	{
		srand(time(NULL)) ;
		center[i] = rand()%2 ;
		if(center[i])
		{
			start.append(i) ;
		}
	}
	double cost = get_cost(start) ;
	T = 500 , step = 0.1 ;
	while(T)
	{
		bool new_center[n+1] = {0} ;
		for(int i = 0 ; i < n+1 ; i++)
		{
			new_center[i] = center[i] ;
		}
		vector<int> new_start(start) ;
		
		change(new_start,new_center) ;
		int new_cost = get_cost(start) ;
		if(new_cost < cost)
		{
			cost = new_cost ;
			for(int i = 0 ; i < n+1 ; i++)
			{
				center[i] = new_center[i] ;
			}
			start = new_start ;
		}
		else
		{
			double prob = exp(abs(new_cost-cost)/T) ;
			if(1.0*rand()/RAND_MAX <= prob)
			{
				cost = new_cost ;
				for(int i = 0 ; i < n+1 ; i++)
				{
					center[i] = new_center[i] ;
				}
				start = new_start ;
			}
		}
		T = T - step ;
	}
}
