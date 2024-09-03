.PHONY: dev_linux clean build test

test: build
	ctest --output-on-failure -j --test-dir build

build: build/build.ninja
	cmake --build build

build/build.ninja:
	cmake -B build -S . -G Ninja -DK_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug

clean:
	rm -rf build

dev_linux:
	docker build -t chinmaygarde/kdoth .
	docker run --rm -it -v ${CURDIR}:/src -w /src chinmaygarde/kdoth /bin/bash
