# Rocket-game
Project for Embedded Software for the IoT
![RocketGame](https://github.com/Fip20/RocketGame/blob/master/Presentation/img/RocketGame.png)
## Description
We made a game of a rocket that has to avoid obstacles. The goal of this project is to emulate the classic game that could be found on old phones or arcade machines on the Booster-Pack screen.
The player, then, will have to move the rocket left or right with the joystick of the Booster-pack having to avoid the obstacles represented by the red squares and keep standing on the black squares collecting as many points as possible.
To improve the user experience also we have added visual and audible feedback, in example a green LED while the game is in play and a red LED when you lose. Also once you lose the buzzer sounds a Game Over sound.
The project was done using the boosterpack, MSP432 and ESP32 interconnected with each other.
The ESP32 is connected to the MSP432 and the Booster Pack. Through the ESP32, we retrieve the top global scores, which are saved on the server.
<hr>

* [Video Demo](https://youtu.be/r9LLOkuCrUQ)

* [Presentation PDF](https://github.com/Fip20/RocketGame/blob/master/Presentation/Rocket%20Game%20new.pdf)
* [Presentation PPT](https://github.com/Fip20/RocketGame/blob/master/Presentation/Rocket%20Game%20new.odp)


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
6. [Usage](#usage)
7. [Contact](#contact)

## Project Structure
```
├───src
│   ├───Code
│       |───buzzerController.c  
│       |───buzzerController.h
|       ├───hwInit.c
|       ├───hwInit.h
|       ├───ledController.c
|       ├───ledController.h
|       ├───main.c
|       ├───screenController.c
|       ├───screenController.h
|   ├───EPS32Code.ino
│   ├───scores_server.py
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
3. Arduino_JSON
4. WiFi
5. HTTPClient

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

         <img src="https://github.com/Fip20/RocketGames/blob/master/Presentation/img/include%20option.png">
         
        
    3. Go to Build > Arm Linker > File Search Path as shown in the image

        <img src="https://github.com/Fip20/RocketGames/blob/master/Presentation/img/file%20search%20paths.png">
        


#### ESP32
1. Install the following libraries from the Arduino IDE library manager:
    1. ArduinoJson
    2. WiFi
    3. HTTPClient
2. Modify the credentials for internet access in the `EPS32Code.ino` file

##### Using the APIs
The following table will show the APIs used in this project and the corresponding HTTP requests:
| API | HTTP Request | Description |
| --- | --- | --- |
| https://federico1110.pythonanywhere.com/get_ranking | GET | get ranking scores from remote server |



## Usage
1. Connect the MSP432 to the computer as described in the [installation section](#installation)
2. Connect the ESP32 to the MSP432 as described in the [installation section](#installation)
3. [Build](#hardware-1) and run the project

1. If you haven't already install as described in the [installation section](#installation)
2. Run the project

## Contributors
1. Luca Pierno Pio
2. Federico Iop
3. Konstantinos Zefkilis
4. Filippo Maffei


## References
1. [MSP432P401R Launchpad](https://www.ti.com/tool/MSP-EXP432P401R)
2. [MSP432P401R BoosterPack](https://www.ti.com/tool/BOOSTXL-EDUMKII)
3. [ESP32](https://www.espressif.com/en/products/socs/esp32)
4. [Arduino_JSON](https://arduinojson.org/)
5. [WiFi](https://www.arduino.cc/en/Reference/WiFi)
6. [HTTPClient](https://www.arduino.cc/en/Reference/HTTPClient)
7. [Driverlib](https://www.ti.com/tool/MSPDRIVERLIB)
8. [Grlib](https://www.ti.com/tool/MSP-GRLIB)
9. [Code Composer Studio](http://www.ti.com/tool/CCSTUDIO)
10. [Arduino IDE](https://www.arduino.cc/en/software)
11. [MSP432P401R Launchpad User's Guide](https://www.ti.com/lit/ug/slau356i/slau356i.pdf)
12. [MSP432P401R Launchpad Hardware User's Guide](https://www.ti.com/lit/ug/slau356j/slau356j.pdf)
13. [MSP432P401R Launchpad Software User's Guide](https://www.ti.com/lit/ug/slau356k/slau356k.pdf)
14. [MSP432P401R Launchpad BoosterPack User's Guide](https://www.ti.com/lit/ug/slau356l/slau356l.pdf)
15. [MSP432P401R Launchpad BoosterPack Hardware User's Guide](https://www.ti.com/lit/ug/slau356m/slau356m.pdf)
16. [MSP432P401R Launchpad BoosterPack Software User's Guide](https://www.ti.com/lit/ug/slau356n/slau356n.pdf)
17. [Video demo](https://youtu.be/r9LLOkuCrUQ)

## Contact
1. [Federico Iop](https://github.com/federico1110) [federico.iop@studenti.unitn.it]

2. [Luca Pierno Pio](https://github.com/cipolla02) [lucapio.pierno@studenti.unitn.it]

3. [Filippo Maffei](https://github.com/Fip20) [filippo.maffei-2@studenti.unitn.it]

4. [Konstantinos Zefkilis](https://github.com/Zefkilis2002) [k.zefkilis@studenti.unitn.it]
