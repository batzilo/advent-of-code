FROM debian

RUN apt update
RUN apt install -y clang clang-tidy

# RUN mkdir /toolchain
# RUN cd /toolchain

# RUN apt install -y wget
# RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.17.0/bazelisk-linux-amd64

ARG TAG=advent-of-code

RUN mkdir /$TAG
RUN cd /$TAG

COPY solution.cpp .

RUN clang solution.cpp
RUN clang-tidy solution.cpp
