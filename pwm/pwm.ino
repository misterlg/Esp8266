/*
*********************************************************************************************************
* Define
*********************************************************************************************************
*/

#define PWM 2

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
    analogWriteFreq(1000);
    analogWrite(PWM, 0);
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
    uint16_t i = 0;
    for(i=0; i<=1024; i++)
    {
        analogWrite(PWM, i);
        delay(2);
    }

    for(i=1024; i>0; i--)
    {
        analogWrite(PWM, i);
        delay(2);
    }
}
