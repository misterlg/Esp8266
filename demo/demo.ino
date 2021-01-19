/*
*********************************************************************************************************
* Include
*********************************************************************************************************
*/

#define BLINKER_WIFI
#include <Blinker.h>
#include <ESP8266WiFi.h>

/*
*********************************************************************************************************
* Define
*********************************************************************************************************
*/

#define KEY 0
#define LED 2

#define ON  0
#define OFF 1

/*
*********************************************************************************************************
* Global Variable
*********************************************************************************************************
*/

char auth[] = "64edb0920377";                                                      /* 设备密钥           */
BlinkerButton Button1("btn-led");                                                  /* 新建Button组件     */
BlinkerNumber TEMP("temp");                                                        /* 新建Number组件     */

float temp = 0;                                                                    /* 采集温度           */

/*
*********************************************************************************************************
* 功  能：一键配网
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/10/31
*********************************************************************************************************
*/

void SmartConfig(void)
{
    WiFi.mode(WIFI_STA);
    Serial.println("\r\nWait for Smartconfig");
    WiFi.beginSmartConfig();
    while(1)
    {
        Serial.print(".");
        digitalWrite(LED, ON);
        delay(500);
        digitalWrite(LED, OFF);
        delay(500);
        if (WiFi.smartConfigDone())
        {
            Serial.println("\r\nSmartConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
  
            WiFi.setAutoConnect(true);                                              /* 设置wifi自动连接   */
            break;        
        }
    }
}

/*
*********************************************************************************************************
* 功  能：按键检测
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/10/31
*********************************************************************************************************
*/

uint8_t Key_Detect(void)
{
    static uint8_t counter = 0;
    if(!digitalRead(KEY))
    {
        if(++counter >= 0x60)
        {
            counter = 0;
            return 1;
        }
    }
    else
        counter = 0;
    return 0;
}

/*
*********************************************************************************************************
* 功  能：button回调函数
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/10/31
*********************************************************************************************************
*/

void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    if(state == "on")
    {
        digitalWrite(LED, ON);
        Button1.print("on");    
    }
    else if(state == "off")
    {
        digitalWrite(LED, OFF);
        Button1.print("off");    
    }
    Blinker.vibrate();
}

/*
*********************************************************************************************************
* 功  能：心跳包函数
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/11/10
*********************************************************************************************************
*/
void Heartbeat() {

}

/*
*********************************************************************************************************
* 功  能：模块初始化
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/10/31
*********************************************************************************************************
*/

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Start module");
    
    pinMode(KEY, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(LED, OFF);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
    Serial.println("WiFi connecting...");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(30);
        if(Key_Detect())
        {
            SmartConfig();
            digitalWrite(LED, ON);
        }
    }
    Serial.println("WiFi connected");

    Blinker.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
    Button1.attach(button1_callback);
    BLINKER_DEBUG.stream(Serial);

    Blinker.attachHeartbeat(Heartbeat);

    Serial.println("Start module successful");
}

/*
*********************************************************************************************************
* 功  能：循环调用
* 形  参：None
* 返回值：None
* 备  注：None
* 作  者：LG
* 日  期：2020/10/31
*********************************************************************************************************
*/

void loop()
{
    static uint8_t counter = 0;
    
    Blinker.run();
    
    if(Key_Detect())
    {
        SmartConfig();
        digitalWrite(LED, ON);
    }

    if(++counter == 0xff)
    {
        temp==100 ? temp=0 : ++temp;
        TEMP.print(temp);
    }
}
