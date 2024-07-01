all:
	# echo "+++ Running CMake"
	cmake -S . -B build/
	# echo "+++ Running CMake --build"
	cd build/ ; cmake --build .
	# echo "+++ Running tests"
	cd build/ ; make test

clean:
	rm -rf build/*
