.PHONY: main pdb corner-copies

main:
	mkdir -p bin
	g++ ./*.cpp -o bin/bin

pdb:
	g++ cube.cpp pdb_api.cpp corner.cpp pdbs/pdb_builder.cpp -o pdbs/bin

corner-copies:
	@if [ -z "$(N)" ]; then \
		echo "Usage: make corner-copies N=<count>"; \
		exit 1; \
	fi
	@case "$(N)" in \
		*[!0-9]* ) \
			echo "N must be a non-negative integer"; \
			exit 1; \
			;; \
	esac
	@if [ ! -f ./pdbs/corner.pdb ]; then \
		echo "Missing source file: ./pdbs/corner.pdb"; \
		exit 1; \
	fi
	@i=0; \
	while [ "$$i" -lt "$(N)" ]; do \
		cp ./pdbs/corner.pdb ./pdbs/corner$$i.pdb; \
		i=$$((i + 1)); \
	done
