#include "deploy.h"
#include <bits/stdc++.h>
using namespace std;
struct Edge{
	int from,to,cap,flow,cost;
	Edge(){};
	Edge(int u,int v,int c,int f,int w):from(u),to(v),cap(c),flow(f),cost(w){}
};

struct MCMF{
	static const int INF = 100010;
	static const int FLOW_INF = 5010;
	static const int maxn = 1002;

	int n,m;
	vector<Edge> edges;
	vector<int> G[maxn]; /*need declear maxn*/
	int inq[maxn];
	int d[maxn];
	int p[maxn]; /*last edge?*/
	int a[maxn];
	int require;
	
	int num;

	string filename;

	/* for simple solution*/

	vector<int> _end;
	vector<int> _end_from;
	vector<int> _need;

	vector< vector<int> > path_info;
	vector<int> tmp;

	int server_cost;

	

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
	
	bool BellmanFord(int s,int t,int &flow,int &cost,bool flag){//default : false
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

		if(flag)
		{
			tmp.clear();
		}

		flow += a[t];
		cost +=  d[t] * a[t];
		for(int u=t;u!=s; u=edges [ p[u] ].from ){
			edges[ p[u] ].flow += a[t];
			edges[ p[u]^1 ].flow -= a[t];

			if(flag)
			{
				if(u!=t)
				tmp.push_back(u);
			}
		}

		if(flag)
		{
			reverse(tmp.begin(),tmp.end());
			tmp.back() -= num;
			
			tmp.push_back( a[t] );

			path_info.push_back(tmp);
		}
		return true;

	}

	void reset_flow()
	{
		for(int i=0;i<m;i++)
		{
			edges[i].flow = 0;
		}
	}

	int MincostMaxflow(int s,int t,int &cost,bool flag){
		int flow = 0;
		cost = 0;

		while(BellmanFord(s,t,flow,cost,flag));
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
		int number_of_node,int tot_need
		)
	{
		_end = end;
		_end_from = end_from;
		_need = need;
		
		num = number_of_node;

		init(number_of_node+end.size() + 1);

		for(int i=0;i<end.size();i++)
		{
			AddEdge(number_of_node+ end[i], n-1 , need[i], 0 );
		}
		for(int i=0;i<end.size();i++)
		{
			AddEdge( end_from[i], number_of_node + end[i],need[i],0);
		}

		for(int i=0;i<path_start.size();i++)
		{
			AddEdge(path_start[i],path_end[i],path_flow[i],path_cost[i]);
			AddEdge(path_end[i],path_start[i],path_flow[i],path_cost[i]);
		}
		require = tot_need;

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
		bool flag = false
		)
	{
		//init(number_of_node+1);//we assume that n+1 is the real end
		//int assume_end = number_of_node;
		//build_graph();

		if(start.size()==0)
		{
			return server_cost*_end.size();
		}

		if(!flag)
		{
			path_info.clear();
		}

		make_start(start);
		int cost;
		int flow = MincostMaxflow(start[0],n-1,cost,flag);//n is the end node
		
		if(!flag)
		{
			DeleteEdge(start.size()-1);
			reset_flow();
		}
		
		if(flow < require) return server_cost * _end.size();
		return cost + server_cost * start.size() ;		

	}

	
	void print_path(const vector<int> &start)
	{
		ostringstream os;

		int cost = get_cost(start,true);
		if(cost== server_cost * _end.size() )
		{
			os << _end_from.size() << endl;

			for(int i=0;i<_end_from.size();i++)
			{
				os << _end_from[i]<<' '<<_end_from[i]<<' ' << _need[i] << endl;
			}

			write_result(os.str().c_str(), filename.c_str() );
		}

		else
		{
			//print : path_info.size();
			memset(inq,0,sizeof(inq));
			for(int i=0;i<start.size();i++)
			{
				inq[ start[i] ] = 1;
			}

			os << path_info.size() << endl;

			for(int i=0;i<path_info.size();i++)
			{
				vector<int> &cur = path_info[i];
				bool spaced = false;

				for(int j=0;j<cur.size();j++)
				{
					if( cur.size() > 1 && cur[0] == start[0] && inq[ cur[1] ]  && j == 0 ) continue;
					if(spaced) os<<' ';
					os << cur[i];
					spaced = true;
				}
				os << endl;
			}

			write_result(os.str().c_str(), filename.c_str() );
		}
	}

}worker;


void change(vector<int> &start,bool center[])
{
	int tmp = rand()%3 ;
	if((tmp == 0 || start.size() == 0)&& start.size()!=worker.n)
	{
		int pos = rand()%worker.n ;
		while(center[pos]) pos = rand()%worker.n ;
		center[pos] = 1 ;
		start.push_back(pos) ;
	}
	else if(tmp == 1 && start.size()>1)
	{
		int pos = rand()%(start.size()) ;
		int ele = start[pos] ;
		start.erase(start.begin()+pos) ;
		center[ele] = 0 ;
	}
	else if(tmp == 2)
	{
		int pos2 = rand()%(start.size()) ;
		int ele = start[pos2] ;
		start.erase(start.begin()+pos2) ;
		center[ele] = 0 ;
		
		int pos1 = rand()%worker.n ;
		while(center[pos1]) pos1 = rand()%worker.n ;
		center[pos1] = 1 ;
		start.push_back(pos1) ;
	}
}
void SA()
{
	bool center[worker.n+1] = {0} ;
	vector<int> start ;
	vector<int> best_start;
	for(int i = 0 ; i < worker.n+1 ; i++)
	{
		center[i] = rand()%2 ;
		if(center[i])
		{
			start.push_back(i) ;
		}
	}
	double cost = worker.get_cost(start) ;
	double T = 500 , step = 0.1 ;
	while(T>0)
	{
		bool new_center[worker.n+1] = {0} ;
		for(int i = 0 ; i < worker.n+1 ; i++)
		{
			new_center[i] = center[i] ;
		}
		vector<int> new_start(start) ;
		
		change(new_start,new_center) ;
		int new_cost = worker.get_cost(start) ;
		if(new_cost < cost)
		{
			cost = new_cost ;
			for(int i = 0 ; i < worker.n+1 ; i++)
			{
				center[i] = new_center[i] ;
			}
			start = new_start ;
			best_start = start;
		}
		else
		{
			double prob = exp(abs(new_cost-cost)/T) ;
			if(1.0*rand()/RAND_MAX <= prob)
			{
				cost = new_cost ;
				for(int i = 0 ; i < worker.n+1 ; i++)
				{
					center[i] = new_center[i] ;
				}
				start = new_start ;
			}
		}
		T = T - step ;
	}
	worker.print_path(best_start);
}

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{

	// 需要输出的内容
	//char * topo_file;

	int number_of_node, path_num, cost_num;
	int read_num = 0;
	int server_cost;
	
	sscanf( topo[read_num] ,"%d%d%d", &number_of_node,&path_num, &cost_num );
	read_num++;

	sscanf(topo[read_num], "%d", &server_cost );

	read_num++;

	vector<int> path_start, path_end, path_flow, path_cost;

	for(int i=0;i<path_num;i++)
	{
		int s,t,f,c;
		sscanf(topo[read_num],"%d%d%d%d",&s,&t,&f,&c);
		read_num++;
		path_start.push_back(s);
		path_end.push_back(t);
		path_flow.push_back(f);
		path_cost.push_back(c);
	}

	int tot_need = 0;
	vector<int> end,end_from,need;

	for(int i=0;i<cost_num;i++)
	{
		int s,t,c;
		sscanf(topo[read_num],"%d%d%d",&s,&t,&c);
		read_num++;

		end.push_back(s);
		end_from.push_back(t);
		need.push_back(c);

		tot_need += c;
	}

	worker.build_graph(end, end_from,need,
		path_start, path_end, path_flow, path_cost,
		number_of_node,tot_need);

	worker.filename = filename;
	srand(time(NULL)) ;
	SA();
}
