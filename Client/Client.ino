/*
*********************************************************************************************************
* Include
*********************************************************************************************************
*/

#include <ESP8266WiFi.h>

/*
*********************************************************************************************************
* Define
*********************************************************************************************************
*/

#define STASSID  "ZY"
#define STAPSK   "13570849220"

/*
*********************************************************************************************************
* Const
*********************************************************************************************************
*/

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "192.168.31.150";
const uint16_t port = 8080;

/*
*********************************************************************************************************
* 功  能：模块初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/10/29
*********************************************************************************************************
*/

void setup()
{
    delay(5000);
    Serial.begin(115200);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    pinMode(14, OUTPUT);
}

/*
*********************************************************************************************************
* 功  能：循环调用
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/10/29
*********************************************************************************************************
*/

void loop()
{
    WiFiClient client;
    uint32_t timeout = 0;
    int8_t ch = 0;
  
    Serial.print("connecting to ");
    Serial.print(host);
    Serial.print(":");
    Serial.println(port);

    if(!client.connect(host, port))
    {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return ;
    }

    Serial.println("sending data to server");
    if(client.connected())
    {
        client.println("hello from ESP8266");
    }

    timeout = millis();
    while(!client.available())
    {
        if(millis() - timeout > 5000)
        {
            Serial.println(">>> Client Timeout !");
            client.stop();
            delay(5000);
            return ;
        }
    }
    
    Serial.println("receiving from server");

    while(client.available())
    {
        ch = static_cast<char>(client.read());
        Serial.print(ch);
        switch(ch)
        {
            case '0': digitalWrite(14, LOW); break;
            case '1': digitalWrite(14, HIGH); break;
        }
    }

    Serial.println();
    Serial.println("closing connection");
    client.stop();

    Serial.println("wait 5 sec...");
    delay(5000);
}
