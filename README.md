# Vocal-translator
Project for Embedded Software for the IoT
# Ciccia
## Description
Our project focuses on creating an advanced system that acquires an audio file and then translates it into text. To achieve this, we use a control device called the MSP432P401R Launchpad, equipped with a BoosterPack that establishes communication with a PC via an ESP32 module using the UART protocol. Interestingly, the choice of this particular microcontroller was guided by its reliability and versatility in handling complex audio data streams.
To facilitate wireless connectivity, we implemented a 2.4 GHz connection, enabling efficient data transmission between our device and the PC. This choice is based on the need for reliable and fast wireless communication.
Internal communication between the various components of our device takes place via the UART protocol, ensuring an efficient and synchronised data flow. At the same time, communication with the Google API, which is crucial for text translation, follows the HTTP protocol, exploiting Internet connectivity to access and use online translation services. The main functionalities of our translator include the conversion of voice into text through advanced voice recognition algorithms. Once the text is obtained, the system proceeds with the translation using the Google Translate API, and finally, the resulting translation is displayed clearly and comprehensibly on the device's screen.
This integrated approach, combining hardware and software technologies, allows us to realise an advanced audio translation system that could find practical applications in multilingual contexts, facilitating understanding and communication.
<hr>

* [Video Demo](https://youtu.be)

* [Presentation PDF](https://github.com/Fip20/vocal-translator/blob/main/presentation)
* [Presentation PPT](https://github.com/Fip20/vocal-translator/blob/main/presentation)


## Table of contents
1. [Description](#description)
2. [Table of contents](#table-of-contents)
3. [Project Structure](#project-structure)
4. [Requirements](#requirements)
    1. [Hardware](#hardware)
    2. [Software](#software)
    3. [External libraries and APIs](#external-libraries-and-apis)
5. [Installation](#installation)
    1. [Hardware](#hardware-1)
    2. [Software](#software-1)
    3. [External libraries and APIS](#external-libraries-and-apis-1)
        1. [MSP432](#msp432)
        2. [ESP32](#esp32)
        3. [Google APIS](#Google-apis)
6. [Usage](#usage)
7. [Options](#options)
8. [Info](#info)
9. [Presentation and Demo](#presentation-and-demo)
10. [Contact](#contact)

## Project Structure
```
├───.vscode
├───src
│   ├───extra
│       |───automator.py #open through CLI connection with edgeImpulse
│       |───svm.py # SVM classifier
│   ├───images
│   ├───msp432
│   │   ├───grlib
│   │   └───driverlib
│   └───esp32
└───.gitignore
```

## Requirements
### Hardware
1. MSP432P401R Launchpad
2. MSP432P401R BoosterPack
3. ESP32

### Software
1. Code Composer Studio
2. Arduino IDE

### External libraries and APIs
1. Driverlib
2. Grlib
3. Google APIs
4. Arduino_JSON
5. WiFi
6. HTTPClient

## Installation
### Hardware
1. Connect the BoosterpackMKII to the MSP432
2. Connect the MSP432 to the computer
3. Connect the ES32 to the computer using the USB cable
4. Connect the ESP32 to the MSP432
In order to set up the UART communication we need to the following prelimary steps:
1. Connect pin 16 of the ESP to pin 3.3 of the MSP
2. Connect pin 17 of the ESP to pin 3.2 of the MSP

### Software
1. Clone the repository
2. Open the project in Code Composer Studio, if you don't have it, you can download it [here](http://www.ti.com/tool/CCSTUDIO)
3. Build the project and burn it to the MSP432
4. Open the `ESP32WiFi.ino` file in the Arduino IDE
5. Burn the code to the ESP32

### External libraries and APIS
#### MSP432
1. Download the [driverlib](https://www.ti.com/tool/MSPDRIVERLIB) and [grlib](https://www.ti.com/tool/MSP-GRLIB) libraries
2. Add the libraries to the linker and loader in the project properties as shown below:
    1. Open CCS and go to Projects > Properties
    2. Go to Build > Arm Compiler > Include Options as shown in the image

        <img src="https://github.com/Fip20/vocal-translator/blob/main/src/images/include%20options.png">
        
    3. Go to Build > Arm Linker > File Search Path as shown in the image

        <img src="https://github.com/Fip20/vocal-translator/blob/main/src/images/file%20search%20path.png">
        


#### ESP32
1. Install the following libraries from the Arduino IDE library manager:
    1. ArduinoJson
    2. WiFi
    3. HTTPClient
2. Modify the credentials for internet access in the `ESP32WiFi.ino` file
3. Modify the bearer token as explained in the [next section](#google-apis)

#### Google APIS
##### Connecting to the Google API
The next steps are to be followed in order to get the access token:
1. Create a Google developer account
2. Create a new project on the Google developer dashboard
3. Add a [new application](https://developer.google.com/dashboard/login)
4. Set a new redirect URI for the application
5. Copy the following link on your browser 
    ```
    (https://)
    ```
    The {XXX} need to be changed to the appropriate values:
    1. client_id: the client id of the application
    2. scope: the actions you want to perform in your application. 
    For this project we need the following scopes:
        ```
        user-read-playback-state user-modify-playback-state user-read-currently-playing
        ```
    3. redirect_uri: the redirect uri of the application (it has to be URL encoded)
6. Copy the token from the redict uri
7. Use the token to call the 'curl' command in your terminal
    ```
    curl -H "Authorization: Basic {XXX}=" -d grant_type=authorization_code -d code={XXX} -d redirect_uri={XXX} https://accounts.google.com/api/token
    ```
    The {XXX} need to be changed to the appropriate values:
    1. authorisation: client_id:client_secret (it has to be URL encoded)
    2. code: the token obtained from the redirect uri
    3. redirect_uri: the redirect uri of the application (it has to be URL encoded)
8. Copy the access token from the response
9. Append the access token to the `token` variable in the `ESP32WiFi.ino` file so that it is "Bearer {appended_token}"

##### Using the APIs
The following table will show the APIs used in this project and the corresponding HTTP requests:
| API | HTTP Request | Description |
| --- | --- | --- |
|        e.g. 50|

Volume can be changed from 0 to 100




## Usage
1. Connect the MSP432 to the computer as described in the [installation section](#installation)
2. Connect the ESP32 to the MSP432 as described in the [installation section](#installation)
3. [Build](#hardware-1) and run the project

1. If you haven't already install as described in the [installation section](#installation)
2. Run the project


## Options
<img title="Buttons" alt="Buttons" src="https://github.com/Fip20/vocal-translator/blob/main/src/images/lb-guide.jpg?raw=true">

1. Play <img title="Play" alt="play" height="24" width="24" src="https://github.com/Fip20/vocal-translator/blob/main/src/images/spotify-controls/play.png">
2. Pause <img title="Pause" alt="pause" height="24" width="24" src="https://github.com/Fip20/vocal-translator/blob/main/src/images/spotify-controls/pause.png">
3. Next <img title="Next" alt="next" height="24" width="24" src="https://github.com/Fip20/vocal-translator/blob/main/src/images/spotify-controls/next.png">
3. Previous <img title="Prev" alt="prev" height="24" width="24" src="https://github.com/Fip20/vocal-translator/blob/main/src/images/spotify-controls/prev.png">
4. Volume Up <img title="Volume Up" alt="volume up" height="24" width="24" src="https://github.com/StefanoDalMas/SpotifyDirectManager_MSP432/blob/main/src/images/spotify-controls/volume-up.png">
5. Volume Down <img title="Volume Down" alt="volume down" height="24" width="24" src="https://github.com/StefanoDalMas/SpotifyDirectManager_MSP432/blob/main/src/images/spotify-controls/volume-down.png">

## Info
1. Display Song Name
2. Display Artist
3. Volume info by progress bar
4. Play/pause state by spinning logo

## Contributors
1. Luca Pierno Pio
2. Federico Iop
3. Konstantinos Zefkilis
4. Filippo Maffei

## Presentation and Demo

## References
1. [MSP432P401R Launchpad](https://www.ti.com/tool/MSP-EXP432P401R)
2. [MSP432P401R BoosterPack](https://www.ti.com/tool/BOOSTXL-EDUMKII)
3. [ESP32](https://www.espressif.com/en/products/socs/esp32)
4. [Google APIs](https://cloud.google.com/)
5. [Arduino_JSON](https://arduinojson.org/)
6. [WiFi](https://www.arduino.cc/en/Reference/WiFi)
7. [HTTPClient](https://www.arduino.cc/en/Reference/HTTPClient)
8. [Driverlib](https://www.ti.com/tool/MSPDRIVERLIB)
9. [Grlib](https://www.ti.com/tool/MSP-GRLIB)
10. [Code Composer Studio](http://www.ti.com/tool/CCSTUDIO)
11. [Arduino IDE](https://www.arduino.cc/en/software)
12. [MSP432P401R Launchpad User's Guide](https://www.ti.com/lit/ug/slau356i/slau356i.pdf)
13. [MSP432P401R Launchpad Hardware User's Guide](https://www.ti.com/lit/ug/slau356j/slau356j.pdf)
14. [MSP432P401R Launchpad Software User's Guide](https://www.ti.com/lit/ug/slau356k/slau356k.pdf)
15. [MSP432P401R Launchpad BoosterPack User's Guide](https://www.ti.com/lit/ug/slau356l/slau356l.pdf)
16. [MSP432P401R Launchpad BoosterPack Hardware User's Guide](https://www.ti.com/lit/ug/slau356m/slau356m.pdf)
17. [MSP432P401R Launchpad BoosterPack Software User's Guide](https://www.ti.com/lit/ug/slau356n/slau356n.pdf)
18. [Picture to C code converter](https://www.digole.com/tools/PicturetoC_Hex_converter.php)
19. [Edge Impulse](https://studio.edgeimpulse.com/studio/176434)
20. [Dataset](http://download.tensorflow.org/data/speech_commands_v0.02.tar.gz](http://download.tensorflow.org/data/speech_commands_v0.02.tar.gz))
21. [Video](https://youtu.be/)

## Contact
1. [Federico Iop](https://github.com/federico1110) [federico.iop@studenti.unitn.it]

2. [Luca Pierno Pio](https://github.com/cipolla02) [lucapio.pierno@studenti.unitn.it]

3. [Filippo Maffei](https://github.com/Fip20) [filippo.maffei-2@studenti.unitn.it]

4. [Konstantinos Zefkilis](https://github.com/Zefkilis2002) [k.zefkilis@studenti.unitn.it]
