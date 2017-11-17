/***************************************
# copyright (c) 2017 lynsucceed
# E-mail: lynsucceed@gmail.com
# function: DHT11 Sensor Drive
# data sheets
   int8_t humidity_MSB = Data[0];
   int8_t humidity_LSB = Data[1];
   int8_t temperature_MSB = Data[2];
   int8_t temperature_LSB = Data[3];
   int8_t checkbit = Data[4];
***************************************/
#define DS 5
boolean Init_DHT11(void)
{
    boolean check = false;
    pinMode(DS,OUTPUT);
    digitalWrite(DS,1);    //数据总线复位
    delay(5);
    digitalWrite(DS,0);    //单片机把总线拉低
    delay(20);
    digitalWrite(DS,1);   //20ms后单片机把总线拉高
    delayMicroseconds(40); //等待40us
    pinMode(DS,INPUT);
    if(digitalRead(DS) ==0)  //Dht11响应信号把总线拉低
    {
      delayMicroseconds(80); 
      if(digitalRead(DS) == 1)  // 80us后Dht11响应信号把总线拉高，初始化成功
           {
              check = true;
              delayMicroseconds(80); //等待80us后Dht11开始发送数据
           }
    }
    return  check;
}

int DHT11_readByte()
{
  int bite = 0;
  int data = 0;
  pinMode(DS,INPUT);
  for(int i=1;i<=8;i++)
  {
    while(digitalRead(DS) != 1); //等待50us的低电平开始信号结束 
    {
      //delayMicroseconds(50);
    }
    delayMicroseconds(28);   //高电平持续26~28us读为0
    bite = 0;
    if(digitalRead(DS) == 1)  //高电平持续70us读为1
    {
     bite = 1;  
    }
      while(digitalRead(DS) != 0);
      data<<=1;
      data |= bite;
  }
   return data;
}

//int8_t DHT11_startSensor()
//{                                                                             
//   
//    return Data[5];
//}

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   
}

void loop() {
  // put your main code here, to run repeatedly:
   if( Init_DHT11() ==true)
   {
     int Data[5];
     int m = 0;
     for(int k =1;k<=5;k++)
     {
      Data[m] = DHT11_readByte();
      m++;
     }
     pinMode(DS,INPUT);
     if(digitalRead(DS) ==0)    //数据发送完成后Dht11把总线拉低50us，然后单片机把总线拉高，通信结束
     {
        delayMicroseconds(50);
        pinMode(DS,OUTPUT);
        digitalWrite(DS,1);
     }
     int DHT_check = Data[0]+Data[1]+Data[2]+Data[3];  //校验位
     if(DHT_check == Data[4])
     {
       Serial.println("DHT11 Sensor initializates OK");
       Serial.print("Humidity:");
       Serial.print( Data[0]);
       Serial.println("%");
       Serial.print("Temperature:");
       Serial.print( Data[2]);
       Serial.println("℃");
       Serial.print("checkbit:");
       Serial.println( Data[4]);
       delay(1000);
     }
   }
    else 
   {
    Serial.println("DHT11 Sensor not exist");
    delay(1000);
    }
  
   
}
