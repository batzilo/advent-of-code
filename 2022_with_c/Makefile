all:
	# echo "+++ Running CMake"
	cmake -S . -B build/
	# echo "+++ Running CMake --build"
	cd build/ ; cmake --build .
	# echo "+++ Running tests"
	cd build/ ; make test

cov:
	# echo "+++ Running CMake"
	cmake -S . -B build/
	# echo "+++ Running CMake --build"
	cd build/ ; cmake --build .
	# echo "+++ Running tests with coverage"
	cd build/ ; make test
	lcov --capture --directory build --output-file build/coverage.info
	genhtml build/coverage.info --output-directory build/lcov_out
	echo "\n\n\nTo see the coverage report, point your browser to\nfile://`pwd`/build/lcov_out/index.html"

clean:
	rm -rf build/*
