# Get the base Ubuntu image from Docker Hub
FROM ubuntu:19.10 AS stage1

# Add for lab09
RUN echo "deb http://cz.archive.ubuntu.com/ubuntu eoan main universe" | tee -a  /etc/apt/sources.list

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the g++ compiler
RUN apt-get -y install g++

# Add for lab09
RUN apt-get -y install libtbb-dev

# Copy the current folder contains C++ source code to the Docker image under /usr/src
COPY . /parallel_programming/

# Specify the working directory
WORKDIR /parallel_programming/

# Use g++ to compile the main.cpp source file
# RUN g++ -o main ./01_matrix/main.cpp -pthread -O3
# RUN g++ -o main ./02_mandelbrot/main.cpp -pthread -O3
# RUN g++ -o main ./09_tbb/main.cpp -ltbb
RUN g++ -o main ./10_lyapunow/lap.cpp -ltbb

# Run the output program from the previous step
CMD ["./main"]

# To build without generating ppm image files
# docker build --file Dockerfile -t odm_image .
# docker run --rm -it  odm_image:latest

# --> Uncomment for scripts generating ppm file <--
# RUN ./main
# FROM scratch AS export-stage
# COPY --from=stage1 /parallel_programming/*.ppm .

# To build with generating ppm image files
# DOCKER_BUILDKIT=1 docker build --file Dockerfile -t odm_image --output out .