.PHONY: main pdb corner-copies

main:
	mkdir -p bin
	g++ ./*.cpp -o bin/bin

pdb:
	g++ cube.cpp pdb_api.cpp corner.cpp pdbs/pdb_builder.cpp -o pdbs/bin