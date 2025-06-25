#include <stdio.h>
#include <string.h>
#include <ESP8266WiFi.h>

// WiFi router and RasPi server
// const char* my_ssid = "rt500m-d46b42-2";
// const char* my_password = "ef41a05b4cd0f";
// const byte serverIP[] = { 192,168,1,6 };
const char* my_ssid = "tethering-k";
const char* my_password = "qweiop123890";
const byte serverIP[] = { 10,186,34,128 };

WiFiServer server(2000);
WiFiClient client;

void setup()
{
  // PWM 出力ポート
  pinMode(5,OUTPUT);
  analogWrite(5, 63);

  Serial.begin(115200);

  // WiFi アクセスポイントに接続
  Serial.print("\n\nWiFi Connecting ");
  WiFi.begin(my_ssid, my_password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nconnected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    
  uint8_t mac0[6];
  WiFi.macAddress(mac0);
  Serial.printf("WiFi Mac Address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5]);
  
  server.begin();
  Serial.println("Server started");

  // RasPi server に接続して初期データをリクエスト
  client.connect(serverIP, 2000);
  Serial.print("\nRasPi Server Connecting ");

  while( !client.connected() ) {
    // client.connect(serverIP, 2000);
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nconnected and request initial data.");
  // 初期値リクエストコマンド送信
  client.write("lightL\nstart\n.\n.\n.", 19);
  //client.write("lightD\nstart\n.\n.\n.", 19);
  //client.write("lightA\nstart\n.\n.\n.", 19);
  client.stop();
  Serial.println("done.\n\n");
}


String command;
char param[128];
int level;
void loop() {
  WiFiClient client = server.available();
  if (client) {
    command = "";
    while (client.connected()) {
      // client.available()は読み込み可能なバイト数を返す
	    if (client.available()) {
	      // 送信データを1バイト読む
        char c = client.read();
        if (c == '\n') {
          break;
        } else if (c != '\r') {
          command += c;
        }
      }
    }
    int counter=0;
    while (client.connected()) {
	    if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          break;
        } else if (c != '\r') {
          param[counter++] = c;
        }
      }
    }
    param[counter++] = '\n';

    client.stop(); // 接続切断

    Serial.print(command +" : ");
    sscanf(param, "%d", &level);
    Serial.printf("%d\n", level);

    analogWrite(5, level);
  }
}
