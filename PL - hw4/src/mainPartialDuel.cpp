#include "PokeFire.h"
#include "PokeWater.h"
#include "PokeElectric.h"
#include "PokeGrass.h"
#include "PokeFlying.h"

#include <iostream>

#include "Trainer.h"
#include "Tournament.h"

bool duelTrainerTest()
{
	Pokemon *charmander = new PokeFire(0, "Charmander");
	Pokemon *charmeleon = new PokeFire(1, "Charmeleon");
	Pokemon *charizard = new PokeFire(2, "Charizard");

	std::vector<Pokemon *> firePokemons;

	firePokemons.push_back(charmander);
	firePokemons.push_back(charmeleon);
	firePokemons.push_back(charizard);

	Pokemon *squirtle = new PokeWater(3, "Squirtle");
	Pokemon *wartortle = new PokeWater(4, "Wartortle");
	Pokemon *blastoise = new PokeWater(5, "Blastoise");

	std::vector<Pokemon *> waterPokemons;

	waterPokemons.push_back(squirtle);
	waterPokemons.push_back(wartortle);
	waterPokemons.push_back(blastoise);

	Pokemon *bulbasaur = new PokeGrass(6, "Bulbasaur");
	Pokemon *ivysaur = new PokeGrass(7, "Ivysaur");
	Pokemon *venasaur = new PokeGrass(8, "Venasaur");

	std::vector <Pokemon *> grassPokemons;

	grassPokemons.push_back(bulbasaur);
	grassPokemons.push_back(ivysaur);
	grassPokemons.push_back(venasaur);

	Trainer *fire = new Trainer(0, "Fiyah", MAGMA, firePokemons);
	Trainer *water = new Trainer(1, "Watah", OCEAN, waterPokemons);
	Trainer *grass = new Trainer(2, "Grasss", FOREST, grassPokemons);

	Tournament testTournament("TestTournament");

	int firstDuel = testTournament.duelBetween(fire, grass, MAGMA);
	int secondDuel = testTournament.duelBetween(water, fire, OCEAN);
	int thirdDuel = testTournament.duelBetween(grass, water, FOREST);

	if (firstDuel != 1)
	{
		std::cout << "Problem in Duel 1" << std::endl;
	}
	if (secondDuel != 1)
	{
		std::cout << "Problem in Duel 2" << std::endl;
	}
	if (thirdDuel != -1)
	{
		std::cout << "Problem in Duel 3" << std::endl;
	}

	return (firstDuel == 1) && (secondDuel == 1) && (thirdDuel == -1);
}

int main(int argc, char** argv)
{
	int totalGrade = 0;

	if(duelTrainerTest())
		totalGrade += 10;

	return totalGrade;
}