# Ubuntu base
FROM ubuntu:20.04

RUN apt-get update
RUN apt-get upgrade -y

# Set locale
RUN apt-get install locales
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    locale-gen
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -y install tzdata

# Install dependencies
RUN apt-get update && apt-get install -y --show-progress --autoremove \
    build-essential \
    python \
    git \
    less \
    wget \
    bzip2 \
    pkg-config \
    cmake \
    vim \
    libopenmpi-dev \
    libpetsc3.12-dev-common \
    ca-certificates

RUN rm -rf /var/lib/apt/lists/*

COPY . /hpc_turbulence_code

ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

RUN rm -rf /hpc_turbulence_code/build/*

WORKDIR /hpc_turbulence_code/build
