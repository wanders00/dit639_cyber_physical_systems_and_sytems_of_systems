##################################################
# Section 1: Build the application
FROM ubuntu:22.04 as builder

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
        cmake \
        build-essential \
        lcov \
        gzip \
        python3 \
        python3-pip \
        python3.10-venv \
        libopencv-dev 

ADD . /opt/sources
WORKDIR /opt/sources

RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && make test && cp 2024-group13-dit639 /tmp

RUN lcov --directory . --capture --output-file coverage.info && \
    lcov --remove coverage.info '/usr/*' '*/*.hpp' '*/Test*' --output-file coverage.info && \
    lcov --list coverage.info && \
    python3 -m venv venv  && \
    . venv/bin/activate && \
    pip install lcov_cobertura && \
    lcov_cobertura coverage.info --output coverage.xml && \
    deactivate && \
    cp coverage.xml /tmp

##################################################
# Section 2: Create a scratch image containg the coverage report
FROM scratch as copytohost
COPY --from=builder /tmp/coverage.xml .

##################################################
# Section 3: Bundle the application.
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
        libopencv-core4.5 \
        libopencv-highgui4.5 \
        libopencv-imgproc4.5 

WORKDIR /opt
COPY --from=builder /tmp/2024-group13-dit639 .
ENTRYPOINT ["/opt/2024-group13-dit639"]