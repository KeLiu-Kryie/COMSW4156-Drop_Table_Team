# Use a base image with minimal dependencies
FROM ubuntu:jammy

#install general dependencies

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/NewYork


RUN apt-get update && \
    apt-get -y dist-upgrade && \
    apt-get install -y build-essential \
    apt-utils git sudo && \
    rm -rf /var/lib/apt/lists/*RUN 

RUN export CPLUS_INCLUDE_PATH=/usr/local/include/mongocxx/v_noabi:/usr/local/include/bsoncxx/v_noabi:$CPLUS_INCLUDE_PATH

RUN sudo ln -s /usr/local/lib/libmogocxx.so.3.8.0 libmongocxx.so._noabi && \
    sudo ln -s /usr/local/lib/libmongocxx.so._noabi /usr/local/lib/libmongocxx.so

WORKDIR /app

COPY . /app

RUN chmod +x /app/install.sh && /app/install.sh && \
    wget https://github.com/CrowCpp/Crow/releases/download/v1.0+5/crow-v1.0+5.deb && \
    sudo apt-get install ./crow-v1.0+5.deb && \
    make clean && make

# Make the binary executable (if necessary)
RUN chmod +x /app/bin/main

RUN export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Expose the port your web server will be listening on
EXPOSE 80

# Define the command to run your application
CMD ["/app/bin/main", "-p", "80"]
