#include <iostream>
#include <fstream>
#include "Graph.hpp"

using namespace std;

int main(int argc,char* argv[]){
	try {
		if(argc != 2)
			throw std::invalid_argument("Enter only one filename!");
	}catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		return 0;
	}
	const std::string FileName(argv[1]);
	std::ifstream file;
	try{
		file.open(FileName.c_str());
		if(!file.good())
			throw std::runtime_error("Could not open file!");
	}catch(const std::exception& ex){
		cout << ex.what() << endl;
		return 0;
	}
	int numberOfDestinations,numberOfCities,index_1,index_2;
	std::vector<int> temp;
	int tmp;
	file >> numberOfCities;
	file >> numberOfDestinations;
	file >> index_1 >> index_2;

	for (int i = 0; i < numberOfDestinations; i++) {
		file >> tmp;
		temp.push_back(tmp);
	}
	Graph Dijkstra(numberOfCities,index_1,index_2,temp);
	for (int i = 0; i < numberOfCities ; i++) {
		for (int j = 0; j < numberOfCities; j++) {
			file >> tmp;
			if (tmp != 0) {
				Dijkstra.insert(i,j,tmp);
			}
		}
	}
	file.close();
	Dijkstra.sortPaths();
 	return 0;
}