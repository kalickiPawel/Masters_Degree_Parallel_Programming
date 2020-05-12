# Get the base Ubuntu image from Docker Hub
FROM ubuntu:19.10

# Add for lab09
RUN echo "deb http://cz.archive.ubuntu.com/ubuntu eoan main universe" | tee -a  /etc/apt/sources.list

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the g++ compiler
RUN apt-get -y install g++

# Add for lab09
RUN apt-get -y install libtbb-dev

VOLUME [ "/data" ]

# Copy the current folder contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/parallel_programming

# Specify the working directory
WORKDIR /usr/src/parallel_programming

# Use g++ to compile the main.cpp source file
# RUN g++ -o main ./01_matrix/main.cpp -pthread -O3
RUN g++ -o main ./02_mandelbrot/main.cpp -pthread -O3
# RUN g++ -o main ./09_tbb/main.cpp -ltbb

# Run the output program from the previous step
CMD ["./main"]
