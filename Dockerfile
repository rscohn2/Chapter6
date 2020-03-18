FROM ubuntu:18.04 AS builder
WORKDIR /project
RUN apt-get update && \
    apt-get install -y cmake git vim gcc g++ wget software-properties-common \
            libgtk2.0-0 libxxf86vm1 libsm6 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Installing latest GCC compiler (version 9) for best vectorization
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update && \
    apt-get install -y gcc-9 g++-9 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9

# Installing Intel compilers since they give the best vectorization among compiler vendors
# Also installing Intel Advisor to look at vectorization performance
RUN wget -q https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS-2023.PUB
RUN apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS-2023.PUB
RUN rm -f GPG-PUB-KEY-INTEL-SW-PRODUCTS-2023.PUB
RUN echo "deb https://apt.repos.intel.com/oneapi all main" >> /etc/apt/sources.list.d/oneAPI.list
RUN echo "deb [trusted=yes arch=amd64] https://repositories.intel.com/graphics/ubuntu bionic main" >> /etc/apt/sources.list.d/intel-graphics.list
RUN apt-get update && \
     apt-get install -y \
             intel-hpckit-getting-started \
             intel-oneapi-common-vars \
             intel-oneapi-common-licensing \
             intel-oneapi-dev-utilities \
             intel-oneapi-icc \
             intel-oneapi-ifort \
             intel-oneapi-advisor && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

SHELL ["/bin/bash", "-c"]

RUN groupadd chapter6 && useradd -m -s /bin/bash -g chapter6 chapter6

WORKDIR /home/chapter6
RUN chown -R chapter6:chapter6 /home/chapter6
USER chapter6

RUN git clone --recursive https://github.com/essentialsofparallelcomputing/Chapter6.git

ENTRYPOINT ["bash"]