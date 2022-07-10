# k.h Development Environment for Linux
# =====================================
#
# Launch using the Makefile at the root of the project.
#

FROM ubuntu:latest
MAINTAINER Chinmay Garde <chinmaygarde@gmail.com>

# Update dependencies.
RUN apt update

# Install dependencies.
RUN apt install -y      \
        build-essential \
        cmake           \
        ninja-build     \
        git
