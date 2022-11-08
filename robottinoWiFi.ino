#include <SoftwareSerial.h> //richiama la classe SoftwareSerial.h

#define rxPin 10
#define txPin 8
// Set up a new SoftwareSerial object
SoftwareSerial serialModuloWIFI =  SoftwareSerial(rxPin, txPin) //creo un oggetto della classe SoftwareSerial passando i parametri nel costruttore
//metodi: begin(), available(), print(), println()
    
String check4answer(){ //restituisce la risposta del modulo WiFi
    String str = "";
    while (serialModuloWIFI.available() > 0) { //esegue i comandi finché ci sono dei dati dal modulo WiFi
        char c = serialModuloWIFI.read(); //ricevo i dati provienti dal modulo WiFi
        Serial.print(c); //debug
        str += String(c);
    }
    return str; //quando non ci sono più dati restituisce il valore di str
}

//Invia comandi al modulo WiFi che si chiama esp01
//riceve il comando da inviare (nella string cmd)
//aspetta e restituisce la risposta del modulo WiFi
String esp01cmd(String cmd) {
  Serial.println("sending: " + cmd);//stampo sul monitor seriale il comando che viene inviato al modulo WiFi(funzione di debug)
  serialModuloWIFI.println(cmd); //invio i dati dall'Arduino al modulo WiFi (in questo caso il comando)
  delay(10);
  String risposta = check4answer();//verifico la risposta del modulo WiFi
  return risposta;//restituiso la risposta del modulo WiFi
}

void setup()  {
    // Define pin modes for TX and RX
    // pinMode(rxPin, INPUT);
    // pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    serialModuloWIFI.begin(115200); //configuro la velocità di trasmissione dei dati
    Serial.begin(9600);
    delay(1000);
    
    //comandi AT per configurare il modulo WiFi
    esp01cmd("AT");
    delay(1000);
    
    esp01cmd("AT+CWMODE=2");//seleziona la modalità Access Point
    delay(1000);
    
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4");//impostiamo SSID e password dell'Access Point
    delay(1000);
    
    esp01cmd("AT+CIFSR"); //mostra l'indirizzo IP dell'Access Point
    esp01cmd("AT+CIPMUX=1"); //permette una connessione alla volta
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void loop() {
    Serial.println("loop...");
    //AT+CWLIF restituisce l'IP del client connesso al modulo WiFi
    //loop che persiste fino alla connessione di un client
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") {//quando il client è connesso è presente il suo indiirizzo IP 
      Serial.println("no connections so far... still waiting");
      delay(1000);
    }

    Serial.println("Connection from remote device was Established!!!");
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server                   
    
//    String str = ;    
//    Serial.println("received: "+esp01cmd("AT+CWLIF").substring(11,18));
    delay(3000);
    while(true) {
      String str = serialModuloWIFI.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str.substring(startOfSTR));
        //Serial.println("Received: "+str);
        //Serial.println(startOfSTR);
      }
    }

}
