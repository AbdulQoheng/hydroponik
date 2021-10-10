#include <ESP8266WiFi.h>
#include "SoftwareSerial.h"

//variabel serial (Rx, Tx) (D6, D7)
SoftwareSerial DataSerial(12, 13);

const char* ssid     = "AL-AZIZ"; //nama wifi
const char* password = "bismillah123"; //password
const char* host = "192.168.0.104"; //IP PC

unsigned long previousMillis = 0;
const long interval = 1000;

String arrData[7];
String tombol = "1";

String s_dht_suhu = "";
String s_dht_kelembapan = "";
String s_suhu_air = "";
String s_tds_sensor = "";
String s_ph_sensor = "";
String s_volume_air = "";

void setup() {
  Serial.begin(9600);
  DataSerial.begin(9600);
  delay(10);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

}

void loop() {
  arrData[0] = "";
  arrData[1] = "";
  arrData[2] = "";
  arrData[3] = "";
  arrData[4] = "";
  arrData[5] = "";
  arrData[6] = "";



  // konf millis
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    String data = "";
    while (DataSerial.available() > 0) {
      data += char(DataSerial.read());
    }

    data.trim();

    if (data != "") {
      //format data suhu_udara#kelembapan_udara#suhu_air#ppm_air#ph_air#volume_air

      int index = 0;
      for (int i = 0; i <= data.length(); i++) {
        char delimiter = '#';
        if (data[i] != delimiter) {
          arrData[index] += data[i];
        } else {
          index++;
        }

      }
      s_dht_suhu = arrData[0];
      s_dht_kelembapan = arrData[1];
      s_suhu_air = arrData[2];
      s_tds_sensor = arrData[3];
      s_ph_sensor = arrData[4];
      s_volume_air = arrData[5];
      
      kirim_server();
    }

    //minta ke arduino
    DataSerial.println("Minta#" + tombol);
    delay(1000);

  }


}

void kirim_server() {

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {

    return;
  }

  // We now create a URI for the request
  //hydroponik/api/sensor?password=hydroponik342&suhu_udara="+ arrData[0] +"&kelembapan_udara="+ arrData[1] +"&suhu_air="+ arrData[2] +"&ppm_air="+ arrData[3] +"&ph_air="+ arrData[4] +"&volume_air="+ arrData[5]
  String url = "/hydroponik/api/sensor?password=hydroponik342&suhu_udara="+s_dht_suhu+"&kelembapan_udara="+s_dht_kelembapan+"&suhu_air="+s_suhu_air+"&ppm_air="+s_tds_sensor+"&ph_air="+s_ph_sensor+"&volume_air="+s_volume_air+"";

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {

      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
//    Serial.println(line);
  }



  delay(10000);
}
