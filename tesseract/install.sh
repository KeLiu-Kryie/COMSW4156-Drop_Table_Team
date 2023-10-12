#!/usr/bin/bash
sudo apt install tesseract-ocr libtesseract-dev g++ autoconf automake libtool pkg-config libpng-dev libjpeg8-dev libtiff5-dev zlib1g-dev libwebpdemux2 libwebp-dev libopenjp2-7-dev libgif-dev libarchive-dev libcurl4-openssl-dev libleptonica-dev
mkdir tessdata && export TESSDATA_PREFIX=$(pwd)/tessdata/ && git clone git@github.com:tesseract-ocr/tessdata_fast.git $(pwd)/tessdata/



