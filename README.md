# vocal-translator
# SpotifyDirectManager_MSP432
Project for Embedded Software for the IoT

## Description
Our project is a Spotify track controller. It allows the user to control the Spotify player on his/her smartphone from a remote device. The remote device is a MSP432P401R Launchpad with a BoosterPack which communicates with the smartphone via Spotify APIs. To do so, we connected our controller to a 2.4 GHz network. Communication between components of our device occurs via UART whereas communication with the Spotify APIs follows the HTTP protocol. Amongst the various features the user will be able to play/pause the song, skip to the next/previous song, change the volume, by using the buttons of the BoosterPack and see the current song name and artist. In addition, we implemented a feature that allows the user to change the volume by rapidly moving the controller up or down which is done thanks to the accelerometer along the Z axis.
<hr>

## Connessione ESP32
Connettere un modulo ESP32 a un computer e a una MSP432 per consentire la connessione Wi-Fi e l'invio di richieste http tramite i seguenti passaggi:

*Installare l'IDE di sviluppo per ESP32, nel nostro caso Arduino IDE.

*Collegare fisicamente l'ESP32 al computer tramite cavo micro-USB.

*Sviluppare il codice per l’ESP32, includendo le librerie necessarie per la gestione del Wi-Fi e delle richieste HTTP.

*Il codice per connettersi a una rete Wi-Fi con un ESP32 è il seguente:

```c
cppCopy code#include <WiFi.h>
const char *ssid = "Nome della tua rete";
const char *password = "Password della tua rete";

void setup() {
  Serial.begin(115200);

  // Connessione Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione al Wi-Fi in corso...");
  }
  Serial.println("Connessione Wi-Fi stabilita");
}

void loop() {
  // Il tuo codice qui
}
```

* Collegamento dell'ESP32 alla MSP432:Collega fisicamente l'ESP32 alla MSP432 tramite UART

*Sviluppo del codice per la MSP432 per interagire con l'ESP32. 

*Gestione delle richieste HTTP:Sul nostro ESP32, usiamo una libreria HTTP, come ad esempio ESPAsyncWebServer, per gestire le richieste HTTP. Crea un endpoint HTTP per ricevere e gestire le richieste dalla MSP432.Questi sono passaggi generali, e la configurazione esatta può variare a seconda delle tue specifiche esigenze e dell'hardware utilizzato. 


