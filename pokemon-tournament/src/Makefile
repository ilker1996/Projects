CPPFLAGS=-ansi -pedantic -g -O0 -c
LDFLAGS=-ansi -g -O0 

partial: ; g++ $(LDFLAGS) mainPartial.cpp Pokemon.cpp PokeFire.cpp PokeWater.cpp PokeElectric.cpp PokeGrass.cpp PokeFlying.cpp -o PokePartial;
partialDuel: ; g++ $(LDFLAGS) mainPartialDuel.cpp TrainerCenter.cpp Tournament.cpp Trainer.cpp Pokemon.cpp PokeFire.cpp PokeWater.cpp PokeElectric.cpp PokeGrass.cpp PokeFlying.cpp -o PokePartialDuel;
single: ; g++ $(LDFLAGS) mainSingleTournament.cpp TrainerCenter.cpp Tournament.cpp Trainer.cpp Pokemon.cpp PokeFire.cpp PokeWater.cpp PokeElectric.cpp PokeGrass.cpp PokeFlying.cpp -o PokeSingle;
multi: ; g++ $(LDFLAGS) mainMultipleTournaments.cpp TrainerCenter.cpp Tournament.cpp Trainer.cpp Pokemon.cpp PokeFire.cpp PokeWater.cpp PokeElectric.cpp PokeGrass.cpp PokeFlying.cpp -o PokeMulti;
memory: ; g++ $(LDFLAGS) mainMultipleTournaments.cpp TrainerCenter.cpp Tournament.cpp Trainer.cpp Pokemon.cpp PokeFire.cpp PokeWater.cpp PokeElectric.cpp PokeGrass.cpp PokeFlying.cpp -o PokeMulti;

clean: ; rm -rf PokePartial; rm -rf PokeTournamentSingle; rm -rf PokeTournamentMultiple;
