#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>


class Graph{

	public:	
		Graph(size_t,int,int,std::vector<int>);
		void insert(int,int,int);
		void printGraph();
		std::vector<int> shortestPath(int warehouse);
		size_t PathVector();
		void sortPaths();
		struct Path{
			Path(int w1,int w2,int dest)
			{
				weight1 = w1;
				weight2 = w2;
				destination = dest;
				difference = w1 - w2;
			};
			int weight1;
			int weight2;
			int destination;
			int difference;
		};
		
		static bool sortByDiff(Path &lhs,Path &rhs){return lhs.difference < rhs.difference;}
		static bool sortByDestination(Path &lhs,Path &rhs){return lhs.destination < rhs.destination;}
		struct Node{
			Node(int x,int y)
			{
				dest = x ;
				weight = y;
			};
			int dest;
			int weight;
		};
		class Compare
		{
			public:
				bool operator() (Node &a,Node &b){
					return a.weight > b.weight;
				}
		};
		std::vector<std::vector<Node> > AdjList;
		std::vector<int> destinations;
		std::vector<Path> pathvector;
		
		int warehouse1;
		int warehouse2;
		size_t size;
		size_t Destsize;





};














#endif