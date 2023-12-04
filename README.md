# vocal-translator
Project for Embedded Software for the IoT

## Description
Our project focuses on creating an advanced system that acquires an audio file and then translates it into text. To achieve this, we use a control device called the MSP432P401R Launchpad, equipped with a BoosterPack that establishes communication with a PC via an ESP32 module using the UART protocol. Interestingly, the choice of this particular microcontroller was guided by its reliability and versatility in handling complex audio data streams.
To facilitate wireless connectivity, we implemented a 2.4 GHz connection, enabling efficient data transmission between our device and the PC. This choice is based on the need for reliable and fast wireless communication.
Internal communication between the various components of our device takes place via the UART protocol, ensuring an efficient and synchronised data flow. At the same time, communication with the Google API, which is crucial for text translation, follows the HTTP protocol, exploiting Internet connectivity to access and use online translation services. The main functionalities of our translator include the conversion of voice into text through advanced voice recognition algorithms. Once the text is obtained, the system proceeds with the translation using the Google Translate API, and finally, the resulting translation is displayed clearly and comprehensibly on the device's screen.
This integrated approach, combining hardware and software technologies, allows us to realise an advanced audio translation system that could find practical applications in multilingual contexts, facilitating understanding and communication.
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


