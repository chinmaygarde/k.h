# Just sets up the default Linux development environment using the Dockerfile
# provided.
#
# To build the project, refer to the CMakeLists.txt
#

dev_linux:
	docker build -t chinmaygarde/kdoth .
	docker run --rm -it -v ${CURDIR}:/src -w /src chinmaygarde/kdoth /bin/bash
