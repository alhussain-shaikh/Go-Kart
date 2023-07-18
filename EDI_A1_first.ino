#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<Wire.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class 
MFRC522::MIFARE_Key key;

ESP8266WebServer server (80);
const char* ssid = "AKSHAY";
const char* password = "abcdefghijk";

String tag;
String page = "";

int p1=0,p2=0,p3=0,p4=0;
int c1=0,c2=0,c3=0,c4=0;
double total = 0;
int count_prod = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

WiFi.begin(ssid, password);
Serial.println("Connecting to wifi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }
  Serial.println("Wifi Connected");
  Serial.println(WiFi.localIP());
  delay(1500);

  Serial.println("Welcome to GoCart");
  Serial.println("Happy Shopping");

  server.on("/", []()
  {
    page = "<html><head><title>E Cart using IoT</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color:  #3498db ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Welcome to GO CART</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>ITEMS</th><th>QUANTITY</th><th>COST</th></tr><tr><td>NesCafe</td><td>"+String(p1)+"</td><td>"+String(c1)+"</td></tr>";
    page += "<tr><td>OREO</td><td>"+String(p2)+"</td><td>"+String(c2)+"</td></tr><tr><td>Nirma Powder(1KG)</td><td>"+String(p3)+"</td><td>"+String(c3)+"</td>";
    page += "</tr><tr><td>Salt(200g)</td><td>"+String(p4)+"</td><td>"+String(c4)+"</td></tr><tr><th>Grand Total</th><th>"+String(count_prod)+"</th><th>"+String(total)+"</th>";
    page += "</tr></table><br><input type=\"button\" name=\"Pay Now\" value=\"Pay Now\" style=\"width: 200px;height: 50px\"></center></body></html>";
    page += "<meta http-equiv=\"refresh\" content=\"2\">";
    server.send(200, "text/html", page);
    });
  server.begin();  

}


void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if(tag=="925712599"){
      Serial.println("Product name : NesCafe ") ;
      Serial.println("Product price : 35 Rs");
      p1++;
      total += 35 ;
      count_prod++;
      Serial.println("Nescafe added to the Cart");
    }
    if(tag=="20813018286"){
      Serial.println("Product name : OREO ") ;
      Serial.println("Product price : 10 Rs");
      p2++;
     
      total += 10 ;
      count_prod++;
      Serial.println("OREO added to the Cart");
    }
    if(tag=="7316765152"){
      Serial.println("Product name : Nirma Powder(1KG) ") ;
      Serial.println("Product price : 80 Rs");
      p3++;
    
      total += 80 ;
      count_prod++;
      Serial.println("Nirma Powder(1KG) added to the Cart");
    }
     if(tag=="24923979152"){
      Serial.println("Product name : Salt(200g) ") ;
      Serial.println("Product price : 25 Rs");
      p4++;
      total += 25 ;
      count_prod++;
      Serial.println("Salt(200g) added to the Cart");
    }

     c1=p1*35.00;
    c2=p2*10.00;
    c3=p3*80.00;
    c4=p4*25.00;
    
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
   
  }
   server.handleClient();
}
