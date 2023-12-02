# Use a base image with minimal dependencies
FROM ubuntu:latest

#install general dependencies

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/NewYork

RUN apt-get update && \
    apt-get -y dist-upgrade && \
    apt-get install -y build-essential \
    apt-utils git sudo

# rm -rf /var/lib/apt/lists/*RUN 

# Set the working directory inside the container
WORKDIR /app

# Copy your compiled binary to the container
COPY . /app

RUN chmod +x /app/install.sh && /app/install.sh && \
    wget https://github.com/CrowCpp/Crow/releases/download/v1.0+5/crow-v1.0+5.deb && \
    sudo apt-get install ./crow-v1.0+5.deb && \
    make clean && make

# Make the binary executable (if necessary)
RUN chmod +x /app/bin/main

# Expose the port your web server will be listening on
EXPOSE 80

# Define the command to run your application
CMD ["/app/bin/main", "-p", "80"]
