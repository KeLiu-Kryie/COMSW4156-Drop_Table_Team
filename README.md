# Drop Table Team
## COMSW4156-Software-Group

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

This project uses the mongocxx libraries as well as boost. To install all mongocxx dependencies, go into the project folder and run the following command: `sudo ./setup-mongocxx`. To install boost run the command `sudo apt-get install libboost-all-dev`.

This project uses Crow to set up an HTTP server to host the service. You can download the latest release of Crow [here](https://github.com/CrowCpp/Crow/releases/tag/v1.0+5) - the `.deb` version is for Ubuntu. Install with: `sudo apt install ./crow-v1.0+5.deb`.

This project uses libcurl to perform REST operations. Install this with: `sudo apt-get install libcurl4-openssl-dev`.

This project uses nlohmann json to serialize and deserialize data into json strings.Install this with: 'sudo apt-get install nlohmann-json3-dev'.

## Getting Started
1. In the project directory, run `make && ./bin/main` to compile the source code and run the server
2. In your web browser, visit [http://localhost:18080/translate/?tbt=hello&tl=jp&fl=en](http://localhost:18080/translate/?tbt=hello&tl=jp&fl=en)

For accessing translation, use the following query parameters:
 - `tbt` = The to-be-translated text
 - `tl` = The language to translate to
 - `fl` = The source language of `tbt`

## Sources:

### Crow
* [Crow Github](https://github.com/CrowCpp/Crow)

* [Crow Tutorial](https://crowcpp.org/master/getting_started/your_first_application/)

* [Query strings](https://crowcpp.org/master/guides/query-string/)

### Curl
* [Saving CURL response](https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c)

### Datastore
* [MongoDB Atlas Tutorials](https://mongocxx.org/mongocxx-v3/tutorial/)

* [MongoDB Connection String](https://www.mongodb.com/docs/guides/atlas/connection-string/)

* [MongoDB C++ Driver](https://www.mongodb.com/docs/drivers/cxx/)

### API
* [baidufanyi-API](https://fanyi-api.baidu.com/doc/11)

### Environment
* [Makefile Implementation](https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/Makefile)

* [Setup dependencies file source](https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/setup-mongocxx) -- Modifications were made based on different versions of the library.

## Translation API doc
### Languages list:
| Supported Languages | Code |
| :-----| :---- |
| English	| en |
| Japenese | jp |
| Korean | kor |
| French | fra | 
| Spanish	| spa |
| Thai | th |
| Arabic | ara |
| Russian | ru | 
| Portuguese	| pt |
| German | de |
| Italian | it |
| Greek | el | 
| Dutch	| nl |
| Polish | pl |
| Italian | it |
| Bulgarian | bul | 
| Estonian | est |
| Danish | dan |
| Finnish | fin |
| Czech | cs |
| Romanian | rom |
| Slovenia | slo |
| Swedish | swe |
| Hungarian | hu |
| Simplified Chinese | zh |
| Traditional Chinese | cht |
| Vietnamese | vie |

automatic detection : 'auto' can only be used in source language.

### Error code list:
| Error Code | Meaning | Solution |
| :-----| :---- | :---- |
| 0 | Success  |  |
| 52001 | Request timeout | Retry |
| 52002 | System error | Retry |
| 52003 | Unauthorized user | Please check whether your appid is correct or whether the service is enabled |
| 54000 | The required parameter is null | Check whether parameters are not transmitted |
| 54001 | Signature error | Please check your signature generation method |
| 54003 | Limited access frequency | Please reduce the frequency of your calls |
| 54004 | Insufficient account balance | Please go to the Admin console to top up your account |
| 54009 | Language detection failure | Not in the range of languages supported |
| 58000 | The IP address of the client is invalid | Check whether the IP address filled in the personal information is correct, you can go to the management control platform to modify, IP restrictions, IP can be left blank  |
| 58002 | The service is currently closed | Please go to the administrative console to start the service |


---
