#ifndef POKEMON_H
#define POKEMON_H

#include <string>

using namespace std;

class Pokemon {
	private:
		// Add private members, methods and constructors here as you need
		string name;
		string type;
		int Evolvecount;
		int Xp;
	public:
		// Do NOT make any modifications below
		mutable int t_ref_cnt;	// stores how many trainers have this pokemon
		Pokemon(const string&, const string&, int);
		Pokemon(const Pokemon&);
		~Pokemon();
		const string& getName() const;
		bool operator>>(const Pokemon&);
		friend Pokemon operator&(Pokemon&, Pokemon&);
		Pokemon& operator=(const Pokemon&);
};

#endif
