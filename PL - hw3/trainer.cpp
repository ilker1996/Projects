#include "trainer.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;



Trainer::Trainer(const string& name , const Pokemon& pokemon, const int min[3], const int max[3]){
	this -> name = name;
	firstpoke  = new Pokemon(pokemon);
	pokemons.push_back(&pokemon);
	for(int i= 0; i < 3;i++){
		this -> min[i] = min[i];
		this -> max[i] = max[i];
	}

}


Trainer::Trainer(const Trainer& rhs){
	name = rhs.name;
	if(firstpoke != NULL){
		firstpoke = new Pokemon(*(rhs.firstpoke));
		for(unsigned int i = 0; i < (rhs.pokemons).size() ; i++){
			pokemons[i] = rhs.pokemons[i];
		}
	}
	for(int i=0;i<3 ;i++){
		this -> min[i] = rhs.min[i];
		this -> max[i] = rhs.max[i];
	}

}
void Trainer::clearVectorContents(std::vector<const Pokemon*>& vec) {    
	for (unsigned int i=1; i<vec.size(); i++){
		delete vec[i];
		vec[i] = NULL;          
    }    
}

Trainer::~Trainer(){
	clearVectorContents(pokemons);
	pokemons.clear();
	delete firstpoke;
	firstpoke = NULL;
}


void Trainer::scanRegion(Region& rhs){//????????
	if(rhs.getPokemonCount(min,max) != 0){
		for(int i = min[0]; i <= max[0]; i++){
			for(int j= min[1]; j <= max[1];j++){
				for(int k = min[2];k<=max[2];k++){
					int tmp[3] = {i,j,k};
					if(rhs.getPokemonCount(tmp,tmp) != 0){
						catchPokemon(rhs(i,j,k));
					}
				}
			}
		}
	}
}

ostream& showPokemons(ostream& os, const Trainer& trainer){
	for(unsigned int i = 0; i < (trainer.pokemons).size() ;i++){
		os << trainer.pokemons[i] -> getName() << endl;
	}
	return os;

}