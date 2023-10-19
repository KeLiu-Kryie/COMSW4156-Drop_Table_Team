# Drop Table Team

## COMSW4156-Software-Group

## Team Members Information:

| Name             | UNI     |
| :--------------- | :------ |
| Brendan Fay      | jbf2173 |
| Ke Liu           | kl3554  |
| Siddharth Ananth | sa4287  |
| Tony Faller      | af3370  |

## Project setup

This project uses linux-based commands to setup all the external libraries / apis needed. You would either pull this project on a linux machine or run it using WSL.

Setting up wsl: https://learn.microsoft.com/en-us/windows/wsl/install

Runnning wsl on VSCode: https://code.visualstudio.com/docs/remote/wsl

This project uses the mongocxx libraries as well as boost. To install all mongocxx dependencies, go into the project folder and run the following command: `sudo ./setup-mongocxx`. To install boost run the command `sudo apt-get install libboost-all-dev`.

This project uses Crow to set up an HTTP server to host the service. You can download the latest release of Crow [here](https://github.com/CrowCpp/Crow/releases/tag/v1.0+5) - the `.deb` version is for Ubuntu. Install with: `sudo apt install ./crow-v1.0+5.deb`.

This project uses libcurl to perform REST operations. Install this with: `sudo apt-get install libcurl4-openssl-dev`.

This project uses nlohmann json to serialize and deserialize data into json strings. Install this with: `sudo apt-get install nlohmann-json3-dev`.

This project depends on poppler. Install this with: `sudo apt-get install libpoppler-dev poppler-utils && sudo apt-get install -y libpoppler-cpp-dev`.

## Getting Started

1. In the project directory, run `make && ./bin/main` to compile the source code and run the server
2. In your web browser, visit [http://localhost:18080/translate/?tbt=hello&tl=jp&fl=en](http://localhost:18080/translate/?tbt=hello&tl=jp&fl=en)

For accessing translation, use the following query parameters:

- `tbt` = The to-be-translated text
- `tl` = The language to translate to
- `fl` = The source language of `tbt`

tl and fl should select from the [language list](#language-list) in API odc.

## Endpoints:

### /translate/

- translates text from one language to another and returns output

Required Params:
tbt: String of the text to be translated
tl: Code for languge to be translated to
fl: Code for language to be translated from 

Example successful response: textToBeTranslated: the-quick-fox-jumps-over-the-lazy-brown-dog, toLang: spa, fromLang: en
Translated text: {"from":"en","to":"spa","trans_result":[{"src":"the-quick-fox-jumps-over-the-lazy-brown-dog","dst":"El zorro \u00e1gil se salta al perro marr\u00f3n perezoso"}]}

### /create_user/

- creates a "user" which stores the 10 most recent translations posted

Example successful response: 
Output Message: 6531acf31b61c9766809cac1 (User id)
Output Code: 200

### /delete_user/

- delete user with given id (deletes all translation history)

Required Params:
id: string of id returned from create_use

Example successful response:
Output Message: Delete Successful
Output Code: 200

### /post_translation_to_user/

- puts translation in a database given a user id (returned from create_user)

- Required Params:
tbt: String of the text to be translated
tl: Code for languge to be translated to
fl: Code for language to be translated from
id: string of id returned from create_user

Example successful response: textToBeTranslated: dog, toLang: de, fromLang: en
Translated text: {"from":"en","to":"de","trans_result":[{"src":"dog","dst":"Hund"}]}
Mongo res: Update successful
Mongo code: 200

### /pdf_to_text/

- scans pdf for text in some language and gives the user that text as a string

### /translate_pdf_text/

- translates text in a pdf and returns the trnslation as a string

### /image_to_text/

- scans image for text, similar to /pdf_to_text/

### /translate_image_text/

- translates text in an image and returns the trnslation as a string

### /post_pdf_translation/

- puts pdf translation into database

### /post_image_translation/

- puts pdf translation into database

## Sources:

### Crow

- [Crow Github](https://github.com/CrowCpp/Crow)

- [Crow Tutorial](https://crowcpp.org/master/getting_started/your_first_application/)

- [Query strings](https://crowcpp.org/master/guides/query-string/)

### Curl

- [Saving CURL response](https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c)

### Datastore

- [MongoDB Atlas Tutorials](https://mongocxx.org/mongocxx-v3/tutorial/)

- [MongoDB Connection String](https://www.mongodb.com/docs/guides/atlas/connection-string/)

- [MongoDB C++ Driver](https://www.mongodb.com/docs/drivers/cxx/)

### API
* [baidufanyi-API](https://fanyi-api.baidu.com/doc/11)

### Environment

- [Makefile Implementation](https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/Makefile)

- [Setup dependencies file source](https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/setup-mongocxx) -- Modifications were made based on different versions of the library.

## Translation API doc

### Languages list

| Supported Languages | Code |
| :------------------ | :--- |
| English             | en   |
| Japenese            | jp   |
| Korean              | kor  |
| French              | fra  |
| Spanish             | spa  |
| Thai                | th   |
| Arabic              | ara  |
| Russian             | ru   |
| Portuguese          | pt   |
| German              | de   |
| Italian             | it   |
| Greek               | el   |
| Dutch               | nl   |
| Polish              | pl   |
| Bulgarian           | bul  |
| Estonian            | est  |
| Danish              | dan  |
| Finnish             | fin  |
| Czech               | cs   |
| Romanian            | rom  |
| Slovenia            | slo  |
| Swedish             | swe  |
| Hungarian           | hu   |
| Simplified Chinese  | zh   |
| Traditional Chinese | cht  |
| Vietnamese          | vie  |

automatic detection : 'auto' can only be used in source language.

### Error code list:

| Error Code | Meaning                                 | Solution                                                                                                                                                                   |
| :--------- | :-------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0          | Success                                 |                                                                                                                                                                            |
| 52001      | Request timeout                         | Retry                                                                                                                                                                      |
| 52002      | System error                            | Retry                                                                                                                                                                      |
| 52003      | Unauthorized user                       | Please check whether your appid is correct or whether the service is enabled                                                                                               |
| 54000      | The required parameter is null          | Check whether parameters are not transmitted                                                                                                                               |
| 54001      | Signature error                         | Please check your signature generation method                                                                                                                              |
| 54003      | Limited access frequency                | Please reduce the frequency of your calls                                                                                                                                  |
| 54004      | Insufficient account balance            | Please go to the Admin console to top up your account                                                                                                                      |
| 54009      | Language detection failure              | Not in the range of languages supported                                                                                                                                    |
| 58000      | The IP address of the client is invalid | Check whether the IP address filled in the personal information is correct, you can go to the management control platform to modify, IP restrictions, IP can be left blank |
| 58002      | The service is currently closed         | Please go to the administrative console to start the service                                                                                                               |

---
