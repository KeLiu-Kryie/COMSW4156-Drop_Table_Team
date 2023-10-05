# Drop Table Team
# COMSW4156-Software-Group


## Team Members Information:
| Name | UNI |
| :-----| :---- |
| Brendan Fay	| jbf2173 |
| Ke Liu | kl3554 |
| Siddharth Ananth | sa4287 |
| Tony Faller | af3370 | 

## Project setup 

This project uses linux-based commands to setup all the external libraries / apis needed. You would either pull this project on a linux machine or run it using WSL.

Setting up wsl: https://learn.microsoft.com/en-us/windows/wsl/install

Runnning wsl on VSCode: https://code.visualstudio.com/docs/remote/wsl

This project uses the mongocxx libraries as well as boost. To install all mongocxx dependencies, go into the project folder and run the following command: sudo ./setup-mongocxx. To install boost run the command sudo apt-get install libboost-all-dev.

To run the mongoDB handler and perform get, push, put and post requests type in the command "make run". This will then prompt you for a mongoDB connection string. A tutorial on getting the connection string can be found here:

https://www.mongodb.com/docs/guides/atlas/connection-string/

## Sources:

Makefile implementation: https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/Makefile

Setup dependencies file source (modifications were made based on different versions of the library): https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/setup-mongocxx

MongoDB Atlas tutorials: https://mongocxx.org/mongocxx-v3/tutorial/

MongoDB C++ driver: https://www.mongodb.com/docs/drivers/cxx/
