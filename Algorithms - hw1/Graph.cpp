#include "Graph.hpp"


# define INF   32767

using namespace std;

Graph::Graph(size_t size,int warehouse1,int warehouse2,std::vector<int> destinations){
    this -> Destsize = destinations.size();
	this -> warehouse1 = warehouse1;
	this -> warehouse2 = warehouse2;
	(this -> AdjList).resize(size);
	(this -> destinations).resize(Destsize);
	this -> destinations = destinations;
	this -> size = size;
}


std::vector<int> Graph::shortestPath(int warehouse)
{
 
    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    std::vector<int> dist(size, INF);
 
    // Insert source itself in Set and initialize its
    // distance as 0.
    dist[warehouse] = 0;
    // instantiate a priority queue with the structure and comparison criteria
    // as defined above
    std::priority_queue<Node,std::vector<Node>,Compare > Q;

    // Create the first node as the source and put it into the queue
    Node first(warehouse,0);
    Q.push(first);
    /*for(unsigned int i = 0; i < (this-> size) ; i++)
        Q.push(Node(i,dist[i]));*/

    // While queue is not empty, pick the min node
    // using it's neighbors update the distance of each node that can be reached
    // and insert that node in the queue
    while(!(Q.empty()))
    {
        int node = Q.top().dest;
        Q.pop();
        size_t limit = AdjList[node].size();
        for(unsigned int  i = 0;i < limit ;i++)
        {
           	int destination = ((AdjList[node])[i]).dest;
           	int weight = ((AdjList[node])[i]).weight;
            // Update the distance if it is smaller than the current distance and push to the queue
            if(dist[destination] > (dist[node] + weight)){
            	dist[destination] = dist[node] + weight;             
                Q.push(Node(destination,dist[destination]));
            }
        }

    }
   	return dist;
}

size_t Graph::PathVector(){
	std::vector<int> wr1 = shortestPath(warehouse1);
	std::vector<int> wr2 = shortestPath(warehouse2);
	for(unsigned int i = 0;i < this -> Destsize;i++){
		Path newPath(wr1[destinations[i]],wr2[destinations[i]],destinations[i]);
		pathvector.push_back(newPath);
	}
	std::sort(pathvector.begin(),pathvector.end(),sortByDiff);
	size_t sum = 0;
	for(unsigned int i = 0,j = (this -> Destsize) - 1; i < (this -> Destsize) / 2 || j >= (this -> Destsize) / 2;i++,j--){
		sum += pathvector[i].weight1 + pathvector[j].weight2;
        
 		pathvector[i].weight2 = warehouse1;
		pathvector[j].weight1 = pathvector[j].weight2;
		pathvector[j].weight2 = warehouse2;
	}
	return sum;
}


void Graph::sortPaths(){
	cout << PathVector() << endl;
	std::sort(pathvector.begin(),pathvector.end(),sortByDestination);
	for(unsigned int i = 0; i < this -> Destsize;i++){
		cout << pathvector[i].destination << " " << pathvector[i].weight2 << endl;
		
	}
}



void Graph::insert(int index,int dest,int weight){
	AdjList[index].push_back(Node(dest,weight));
}


void Graph::printGraph(){
	for(unsigned int i = 0; i < this -> size ;i++){
		cout << i << "th index -->" ;
		for(unsigned int j = 0; j < this -> size ;j++){
			cout << "  ||   " << AdjList[i][j].dest << " , " <<  AdjList[i][j].weight;
		}
		cout << endl;
	}
}

