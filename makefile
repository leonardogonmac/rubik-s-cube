.PHONY: main pdb corner-copies

main:
	mkdir -p bin
	g++ ./*.cpp -o bin/bin

pdb:
	g++ cube.cpp pdb_api.cpp corner.cpp pdbs/pdb_builder.cpp -o pdbs/bin

tests:
	for s in samples/$(DIST)/*; do \
		echo "Running $$s"; \
		/usr/bin/time -f "\nTime: %E\nCPU: %P\nMemory: %M KB\n" ./bin/bin $(THREAD_NUM) < $$s > /dev/null; \
	done