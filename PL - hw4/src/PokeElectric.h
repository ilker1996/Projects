#ifndef POKEELECTRIC_H
#define POKEELECTRIC_H

#include "Pokemon.h"

class PokeElectric : public Pokemon
{
public:
	// Add your public methods/attributes here.
	PokeElectric(int pokemonID,const std::string& name);
	void levelUp();
	void attackTo(Pokemon* target,Arena currentArena);
 	void BuffMachine(Arena currentArena) ;
	void Restart() ;
	void special_effect();
	void setElectrified(bool electrified);



protected:
	// Add your protected methods/attributes here.

private:
	// Add your private methods/attributes here.
	
};

#endif