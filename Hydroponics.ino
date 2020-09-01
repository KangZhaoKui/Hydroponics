硬件：ATmega88PA-AU, internal 8MHz;
//5.10增加电量阶段报警功能，从3.3V开始，随着电量的降低，报警时间越长，但水泵还在工作，只是到3V时，只报警，水泵不工作。

#include <LowPower.h>

#define PumpPin      5
#define VoltTestPin  A1
#define BuzzerPin    6
#define Volt_Power   A2

int v;
//float volt;

void setup()
{
	pinMode(PumpPin, OUTPUT);
	pinMode(BuzzerPin, OUTPUT);
	pinMode(Volt_Power,OUTPUT); 
	
	CLKPR = 0x80;
	CLKPR = 0x03;   //Prescaler=8, 1MHz
	analogReference(INTERNAL); //an built-in reference, equal to 1.1 volts
	//Serial.begin(9600);  //注意，分频会影响串口通信
	digitalWrite(BuzzerPin, HIGH);   //开机提示音
	delay(62);     //8分频，0.5S。
	digitalWrite(BuzzerPin, LOW);
}

void loop()
{
	
	//检测电源电压
	pinMode(VoltTestPin, INPUT);
	digitalWrite(Volt_Power, HIGH);
	delay(12);   //电容充电100ms
	v=analogRead(VoltTestPin);
	digitalWrite(Volt_Power, LOW);

	//防止悬空的引脚电压
	pinMode(VoltTestPin, OUTPUT);
	digitalWrite(VoltTestPin,LOW);

	//volt= v*0.00383;     //0.00383=(1.1/1024.0)/390*(390+1000);
			
	if(v>860){              //电源电压高于3.3V
		digitalWrite(PumpPin, HIGH);
		for(int i=0; i<3; i++){      //水泵工作24秒
			LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
		}
		digitalWrite(PumpPin, LOW);
	} else if(v>835){           //电源电压约为3.2V
		for(int j=0; j<2; j++){ alarm();}
		digitalWrite(PumpPin, HIGH);
		for(int i=0; i<3; i++){      //水泵工作24秒
			LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
		}
		digitalWrite(PumpPin, LOW);
	} else if(v>810){       //电源电压约为3.1V
		for(int j=0; j<3; j++){ alarm();}
		digitalWrite(PumpPin, HIGH);
		for(int i=0; i<3; i++){      //水泵工作24秒
			LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
		}
		digitalWrite(PumpPin, LOW);
	} else{       //电源电压低于3.1V
		for(int j=0; j<6; j++){ alarm();}
	}	
	for(int i=0; i<225; i++){    //省电模式，大约30分钟
	    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
	}

}

void alarm(){
	digitalWrite(BuzzerPin, HIGH);
	LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
	digitalWrite(BuzzerPin, LOW);
	LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
}
