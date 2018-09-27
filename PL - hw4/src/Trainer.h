#ifndef TRAINER_H
#define TRAINER_H

#include <string>
#include <vector>

#include "Pokemon.h"
#include "PokeFire.h"
#include "PokeWater.h"
#include "PokeGrass.h"
#include "PokeFlying.h"
#include "PokeElectric.h"

class Trainer
{
public:
	// Public Methods
	Trainer(int trainerID, const std::string& name, Arena favoriteArena, std::vector<Pokemon *>& pokemons);
	~Trainer();

	// Getters

	int getTrainerID() const;
	const std::string& getName() const;
	Arena getFavoriteArena() const;

	// Own Methods
	Trainer(int trainerID, const std::string& name, Arena favoriteArena);
	int Pokemonfight(Pokemon * first,Pokemon * second,Arena currentArena,int turn);
	int Attack(Trainer * low,Arena currentArena);
	std::vector<Pokemon *> pokemons;
	

private:
	int trainerID; 		// Unique
	std::string name;
	Arena favoriteArena;

	

	// Own Attributes
	
};

#endif
