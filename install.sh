#!/usr/bin/bash
function install_general_dependencies() {
        sudo apt-get install -y tesseract-ocr \
                libtesseract-dev g++ autoconf \
                automake libtool pkg-config \
                libpng-dev libjpeg8-dev \
                libtiff5-dev zlib1g-dev \
                libwebpdemux2 libwebp-dev \
                libopenjp2-7-dev libgif-dev \
                libarchive-dev libcurl4-openssl-dev \
                libleptonica-dev libpoppler-dev \
                libglib2.0-dev poppler-utils \
                nlohmann-json3-dev wget \
                libpoppler-cpp-dev cpplint \
                libboost-all-dev
        # mkdir tessdata && export TESSDATA_PREFIX=$(pwd)/tessdata/ # && git clone git@github.com:tesseract-ocr/tessdata_fast.git $(pwd)/tessdata/
}

# Source: https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/setup-mongocxx
# I found the below steps from:
# https://developer.mongodb.com/community/forums/t/problems-trying-to-compile-mongocxx/2084

function install_mongocxx_dependencies() {
  TOP_DIR=$(pwd)
  sudo apt-get install -y libmongoc-dev \
      libbson-dev cmake \
      libssl-dev libsasl2-dev zlib1g-dev
  sudo ldconfig

  wget https://github.com/mongodb/mongo-c-driver/releases/download/1.24.4/mongo-c-driver-1.24.4.tar.gz
  tar xzf mongo-c-driver-1.24.4.tar.gz
  cd mongo-c-driver-1.24.4
  ./configure
  mkdir cmake-build
  cd cmake-build
  cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
  make
  sudo make install

  curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.8.0/mongo-cxx-driver-r3.8.0.tar.gz
  tar -xzf mongo-cxx-driver-r3.8.0.tar.gz
  cd mongo-cxx-driver-r3.8.0/build

  sudo cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DBSONCXX_POLY_USE_MNMLSTC=1 \
      -DCMAKE_INSTALL_PREFIX=/usr/local ..

  sudo cmake --build . --target EP_mnmlstc_core
  cmake --build .
  sudo cmake --build . --target install
  cd $TOP_DIR
  rm -rf mongo-c-driver-1.24.4*
}

function main() {
  install_general_dependencies
  install_mongocxx_dependencies
}

main
