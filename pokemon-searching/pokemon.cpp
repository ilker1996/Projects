#include "pokemon.h"
#include <iostream>

using namespace std;


Pokemon::Pokemon(const string& x, const string& y,int z){
	name = x;
	type = y;
	Evolvecount = z;
	t_ref_cnt = 0;
	Xp = 0;
}

Pokemon::Pokemon(const Pokemon& p){
	name = p.name;
	type = p.type;
	Evolvecount = p.Evolvecount;
	t_ref_cnt = p.t_ref_cnt;
	Xp = p.Xp;
}

Pokemon& Pokemon::operator=(const Pokemon& p){
	name = p.name;
	type = p.type;
	Evolvecount = p.Evolvecount;
	t_ref_cnt = p.t_ref_cnt;
	Xp = p.Xp;
	return *this;
}
const string& Pokemon::getName() const {
	return name;
}
Pokemon::~Pokemon(){}

bool Pokemon::operator>>(const Pokemon& p){
	if(Evolvecount > Xp || Evolvecount == -1)
		return false;
	else{
		name = p.name;
		type = p.type;
		Evolvecount = p.Evolvecount;
		Xp = p.Xp;
		t_ref_cnt = p.t_ref_cnt;
		return true;
	}
}


Pokemon operator&(Pokemon& p1, Pokemon& p2){
	(p1.Xp)++;
	(p2.Xp)++;
	if(p1.type == "fire"){ // P1 = FIRE
		if(p2.type == "water" )
			return p2;
		else if (p2.type == "grass")
			return p1;
		else if (p2.type == "electric")
			return p1;
		else
			return p2;
	}
	else if (p1.type == "water"){
		if(p2.type == "fire" )
			return p1;
		else if (p2.type == "grass")
			return p2;
		else if (p2.type == "electric")
			return p2;
		else
			return p1;
	}
	else if (p1.type == "grass"){
		if(p2.type == "fire" )
			return p2;
		else if (p2.type== "water")
			return p1;
		else if (p2.type == "electric")
			return p1;
		else
			return p2;
	}
	else if (p1.type == "electric"){
		if(p2.type == "fire" )
			return p2;
		else if (p2.type == "water")
			return p1;
		else if (p2.type == "grass")
			return p2;
		else
			return p1;
	}
	else{
		if(p2.type == "fire" )
			return p1;
		else if (p2.type == "water")
			return p2;
		else if (p2.type == "grass")
			return p1;
		else
			return p2;	
	}
}