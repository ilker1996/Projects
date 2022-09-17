#ifndef REGION_H
#define REGION_H

#include "pokemon.h"
#include <iostream>
#include <exception>

using namespace std;

class Region {
	private:
		// Add private members, methods and constructors here as you need
		int min_x;
		int min_y;
		int min_z;
		int max_x;
		int max_y;
		int max_z;
		Pokemon* pokemon;
		Region* right;
		Region* left;
		Region* parent;
		bool isLeaf() const ;
		char wrt;
		void deleteRegion();
		void deleteOperator();
		Pokemon* getPokemon(int,int,int);
		void deletePokemon(int,int,int);

	public:
		// Do NOT make any modifications below
		Region(const int[3], const int[3]);
		Region(const Region&);
		~Region();
		int getMinBorder(char) const;
		int getMaxBorder(char) const;
		void placePokemon(const Pokemon&, int, int, int);
		Pokemon& operator()(int, int, int);
		int getPokemonCount(const int[3], const int[3]) const;
		Region crop(const int[3], const int[3]) const;
		void patch(Region);
		Region& operator=(const Region&);
};

class pokemonException: public exception {
 	virtual const char* what() const throw() {
    		return "There does not exist any pokemon in the given coordinates!";
	}
};

#endif
