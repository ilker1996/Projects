#ifndef POKEWATER_H
#define POKEWATER_H

#include "Pokemon.h"

class PokeWater : public Pokemon
{
public:
	// Add your public methods/attributes here.
	PokeWater(int pokemonID,const std::string& name);
	void levelUp();
	void attackTo(Pokemon* target,Arena currentArena);
 	void BuffMachine(Arena currentArena) ;
	void Restart() ;
	void special_effect();
	void setDrowning(bool drowning);


protected:
	// Add your protected methods/attributes here.

private:
	// Add your private methods/attributes here.
	
};

#endif