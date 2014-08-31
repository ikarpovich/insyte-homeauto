struct{ // Constants
	#define DEV_SPIDER 1
	#define DEV_TEMP1 5
	#define DEV_TEMP2 6
	#define DEV_RELAY1 2
	#define DEV_RELAY2 7
	#define DEV_RELAY3 3
	#define DEV_IN1 4
	#define OUT1	1
	#define OUT2	2
	#define OUT3	3
	#define OUT4	4
	#define IN1		10001
	#define IN2		10002
	#define IN3		10003
	#define IN4		10004
	#define AIN1 40001
	#define AIN2 40002
	#define AIN3 40003
	#define NULL  ((void*)0x00000000)
	#define DEFAULT_FLOOR_TEMP_BATHROOM 30
	#define DEFAULT_FLOOR_TEMP_KITCHEN 31
	#define FLOOR_TEMP_DELTA 1
	#define LOG_START_INDEX 300
	#define TIMER_GUARD 0
	#define TIMER_GUARD_BEEP_UP 1
	#define TIMER_GUARD_BEEP_ERROR 2
	#define TIMER_LIGHT_SENSOR 5
	#define TIMER_LONGCLICK 6
	#define TIMER_INIT 7
	#define LONGCLICK_TIME 70
	#define TIMER_MOVE_BATHROOM 10
	#define TIMER_MOVE_CORRIDOR 11
	#define TIMER_WATER_BATHROOM1 20
	#define TIMER_WATER_BATHROOM2 21
	#define TIMER_WATER_KITCHEN 22
	#define TIMER_LIGHT_LIVINGROOM 23
	#define PHONE1 "+791........"
	#define PHONE2 "+791........"

}defs;

void main(
	int *pTmpInt, // System variable for integers
	float *pTmpFlt, // // System variable for floats
  	int (*pInts), // Public variable for ModBus accesable integers
	float (*pFloats), // Public variable for ModBus accessable floats
	int (pKeyH), // 1-wire Key
	int (pKeyL), // 1-wire Key
	int (*pGetNodeVar)( unsigned short dev_addr, unsigned short reg_addr ), // Read variable from an address
	int (*pSetNodeVar)( unsigned short dev_addr, unsigned short reg_addr, unsigned short val ), // Write variable value to an address
	int (*pGetTimer)( unsigned short TimerNum ), // Get timer value
	int (*pSetTimer)( unsigned short TimerNum, int TimerVal ), // Set timer value
	int (*pReceiveSMS)( const char *smsnum, const char *smstext, int *parInt, float *parFlt ),/* Receive SMS */
	int (*pSendSMS)( const char *smsnum, const char *smstext, int *parInt, float *parFlt ), /* Send sms */
	int (*pSendCom2)( const char *TxData, unsigned short TxLen, const char *RxData, unsigned short RxLen), /* Send data over Com2 */
	int (*pTimeCmp)( unsigned char date, unsigned char month, unsigned char year, unsigned char hour, unsigned char minute, unsigned char second, unsigned char day, unsigned char act ), // Compare time with onboard realtime clock
	float (*pSinf)(float _X),
	float (*pCosf)(float _X),
	float (*pLogf)(float _X),
	float (*pExpf)(float _X),
	float (*pPowf)(float _X1, float _X2),
	float (*pSqrtf)(float _X)
)
{

int put2log[4]={0,0,0,0};
int lastlog[4]={0,0,0,0};
int *LogCounter=&pInts[LOG_START_INDEX];
int *GlobalInit=&pInts[97];


// Switches flags

int *Livingroom1ButtonPressed=&pInts[10]; // Living room 1
int *Livingroom2ButtonPressed=&pInts[11]; // Living room 2
int *Kitchen1ButtonPressed=&pInts[12]; // Kitchen 1
int *Kitchen2ButtonPressed=&pInts[13]; // Kitchen 2
int *ClosetButtonPressed=&pInts[14]; // Closet
int *CorridorButtonPressed=&pInts[15]; // Corridor
int *BathroomButtonPressed=&pInts[16]; // Bathroom
int *StudyButtonPressed=&pInts[17]; // Study
int *Bedroom1ButtonPressed=&pInts[18]; // Bedroom 1 (Main)
int *Bedroom2ButtonPressed=&pInts[19]; // Bedroom 2 (Bed left side)
int *Bedroom3ButtonPressed=&pInts[20]; // Bedroom 3 (Bed right side)

int *Livingroom1LightSensorWorked=&pInts[80];
int *CorridorMoveDetected=&pInts[81];

int *BathroomFloorStatus=&pInts[98];
int *KitchenFloorStatus=&pInts[99];

int *GuardStatus=&pInts[100]; // Alarm status (0 - Off, 1 - Switching on, 2 - Switching on, 3 - On guard, 4 - Switching off, 5 - Alarm)
int *BuzzerStatus=&pInts[101];

// Underfloor heating flags

int *BathroomFloorEnabled=&pInts[102]; // Bathroom underfloor heating
int *KitchenFloorEnabled=&pInts[103]; // Kitchen underfloor heating
//int *BalconyFloorEnabled=&pInts[104]; // Balcony underfloor heating

// Lighting flags

int *Livingroom1LightStatus=&pInts[105];
int *Livingroom2LightStatus=&pInts[106];
int *Kitchen1LightStatus=&pInts[107];
int *Kitchen2LightStatus=&pInts[108];
int *ClosetLightStatus=&pInts[109];
int *CorridorLightStatus=&pInts[110];
int *CorridorLightStatusMove=&pInts[111];
int *CorridorLightStatusMoveEnabled=&pInts[119];
int *BathroomLightStatus=&pInts[112];
int *BathroomLightStatusMove=&pInts[113];
int *BathroomLightStatusMoveEnabled=&pInts[120];
int *BathroomVentStatus=&pInts[114];
int *StudyLightStatus=&pInts[115];
int *Bedroom1LightStatus=&pInts[116];
int *Bedroom2LightStatus=&pInts[117];
int *Bedroom3LightStatus=&pInts[118];

// Underfloor heating settings

float *BathroomFloorTempExpected=&pFloats[0]; // Target temperature (Bathroom)
float *BathroomFloorTempCurrent=&pFloats[1]; // Current temperature (Bathroom)
float *KitchenFloorTempExpected=&pFloats[2]; // Target temperature (Kitchen)
float *KitchenFloorTempCurrent=&pFloats[3]; // Current temperature (Kitchen)

float *BathroomWaterSum1=&pFloats[4];
float *BathroomWaterAvg1=&pFloats[5];
float *BathroomWaterCount1=&pFloats[6];
float *BathroomWaterSum2=&pFloats[7];
float *BathroomWaterAvg2=&pFloats[8];
float *BathroomWaterCount2=&pFloats[9];
float *KitchenWaterSum=&pFloats[10];
float *KitchenWaterAvg=&pFloats[11];
float *KitchenWaterCount=&pFloats[12];

float *LivingroomLightSum=&pFloats[13];
float *LivingroomLightAvg=&pFloats[14];
float *LivingroomLightCount=&pFloats[15];

// Alarm

int KeyIgor=(pKeyH == 0 && pKeyL == 0) ? 1 : 0;
int KeyOlga=(pKeyH == 0 && pKeyL == 0) ? 1 : 0;
int KeyReserve=(pKeyH == 0 && pKeyL == 0) ? 1 : 0;

if(*LogCounter>0 && pInts[LOG_START_INDEX]>0)
{
	lastlog[0]=pInts[LOG_START_INDEX+1+(*LogCounter-1)*4];
	lastlog[1]=pInts[LOG_START_INDEX+1+(*LogCounter-1)*4+1];
	lastlog[2]=pInts[LOG_START_INDEX+1+(*LogCounter-1)*4+2];
	lastlog[3]=pInts[LOG_START_INDEX+1+(*LogCounter-1)*4+3];
}

if((*GlobalInit)==0)
{
	*CorridorLightStatusMoveEnabled=1;
	*BathroomLightStatusMoveEnabled=1;
	*BathroomFloorEnabled=1;
	*KitchenFloorEnabled=1;
	pSetTimer(TIMER_INIT,5000);
}


// == Switches clicks (beginning) ==

// Living room 1

if((pGetNodeVar(DEV_SPIDER,IN1)==1) && *Livingroom1LightStatus==0 && (*Livingroom1ButtonPressed==0))
{
	*Livingroom1LightStatus=1;
	*Livingroom1ButtonPressed=1;
}
if((pGetNodeVar(DEV_SPIDER,IN1)==1) && *Livingroom1LightStatus==1 && (*Livingroom1ButtonPressed==0))
{
	*Livingroom1LightStatus=0;
	*Livingroom1ButtonPressed=1;
}

// Living room 2

if((pGetNodeVar(DEV_SPIDER,IN1)==1) && *Livingroom2LightStatus==0 && (*Livingroom2ButtonPressed==0))
{
	*Livingroom2LightStatus=1;
	*Livingroom2ButtonPressed=1;
}
if((pGetNodeVar(DEV_SPIDER,IN1)==1) && *Livingroom2LightStatus==1 && (*Livingroom2ButtonPressed==0))
{
	*Livingroom2LightStatus=0;
	*Livingroom2ButtonPressed=1;
}

// Kitchen 1

if((pGetNodeVar(DEV_SPIDER,IN2)==1) && *Kitchen1LightStatus==0 && (*Kitchen1ButtonPressed==0))
{
	*Kitchen1LightStatus=1;
	*Kitchen1ButtonPressed=1;
}
if((pGetNodeVar(DEV_SPIDER,IN2)==1) && *Kitchen1LightStatus==1 && (*Kitchen1ButtonPressed==0))
{
	*Kitchen1LightStatus=0;
	*Kitchen1ButtonPressed=1;
}

// Kitchen 2

if((pGetNodeVar(DEV_SPIDER,IN2)==1) && *Kitchen2LightStatus==0 && (*Kitchen2ButtonPressed==0))
{
	*Kitchen2LightStatus=1;
	*Kitchen2ButtonPressed=1;
}
if((pGetNodeVar(DEV_SPIDER,IN2)==1) && *Kitchen2LightStatus==1 && (*Kitchen2ButtonPressed==0))
{
	*Kitchen2LightStatus=0;
	*Kitchen2ButtonPressed=1;
}

// Closet

if((pGetNodeVar(DEV_IN1,IN4)==1) && *ClosetLightStatus==0 && (*ClosetButtonPressed==0))
{
	*ClosetLightStatus=1;
	*ClosetButtonPressed=1;
}
if((pGetNodeVar(DEV_IN1,IN4)==1) && *ClosetLightStatus==1 && (*ClosetButtonPressed==0))
{
	*ClosetLightStatus=0;
	*ClosetButtonPressed=1;
}

// Corridor

if((pGetNodeVar(DEV_SPIDER,IN3)==1) && *CorridorLightStatus==0 && (*CorridorButtonPressed==0))
{
	*CorridorLightStatus=1;
	*CorridorButtonPressed=1;
}
if((pGetNodeVar(DEV_SPIDER,IN3)==1) && *CorridorLightStatus==1 && (*CorridorButtonPressed==0))
{
	*CorridorLightStatus=0;
	*CorridorButtonPressed=1;
}

// Bathroom

if((pGetNodeVar(DEV_RELAY1,IN1)==1) && *BathroomLightStatus==0 && (*BathroomButtonPressed==0))
{
	*BathroomLightStatus=1;
	*BathroomButtonPressed=1;
}
if((pGetNodeVar(DEV_RELAY1,IN1)==1) && *BathroomLightStatus==1 && (*BathroomButtonPressed==0))
{
	*BathroomLightStatus=0;
	*BathroomButtonPressed=1;
}

// Study

if((pGetNodeVar(DEV_RELAY1,IN2)==1) && *StudyLightStatus==0 && (*StudyButtonPressed==0))
{
	*StudyLightStatus=1;
	*StudyButtonPressed=1;
}
if((pGetNodeVar(DEV_RELAY1,IN2)==1) && *StudyLightStatus==1 && (*StudyButtonPressed==0))
{
	*StudyLightStatus=0;
	*StudyButtonPressed=1;
}

// Bedroom 1 (Main)

if((pGetNodeVar(DEV_RELAY1,IN3)==1) && *Bedroom1LightStatus==0 && (*Bedroom1ButtonPressed==0))
{
	*Bedroom1LightStatus=1;
	*Bedroom1ButtonPressed=1;
	pSetTimer(TIMER_LONGCLICK,LONGCLICK_TIME);
}
if((pGetNodeVar(DEV_RELAY1,IN3)==1) && *Bedroom1LightStatus==1 && (*Bedroom1ButtonPressed==0))
{
	*Bedroom1LightStatus=0;
	*Bedroom1ButtonPressed=1;
	pSetTimer(TIMER_LONGCLICK,LONGCLICK_TIME);
}

if(pGetTimer(TIMER_LONGCLICK)==0 && (*Bedroom2LightStatus==1) && ((*Bedroom2ButtonPressed==1) || (*Bedroom3ButtonPressed==1)))
{
	*Bedroom1LightStatus=1;
	pSetTimer(TIMER_LONGCLICK,-1);
}
if(pGetTimer(TIMER_LONGCLICK)==0 && (*Bedroom2LightStatus==0) && ((*Bedroom2ButtonPressed==1) || (*Bedroom3ButtonPressed==1)))
{
	*Bedroom1LightStatus=0;
	pSetTimer(TIMER_LONGCLICK,-1);
}

// Bedroom 2 (bed left side)

if((pGetNodeVar(DEV_SPIDER,IN4)==1) && *Bedroom2LightStatus==0 && (*Bedroom2ButtonPressed==0))
{
	*Bedroom2LightStatus=1;
	*Bedroom2ButtonPressed=1;
	pSetTimer(TIMER_LONGCLICK,LONGCLICK_TIME);
}
if((pGetNodeVar(DEV_SPIDER,IN4)==1) && *Bedroom2LightStatus==1 && (*Bedroom2ButtonPressed==0))
{
	*Bedroom2LightStatus=0;
	*Bedroom2ButtonPressed=1;
	pSetTimer(TIMER_LONGCLICK,LONGCLICK_TIME);
}

if(pGetTimer(TIMER_LONGCLICK)==0 && (*Bedroom1LightStatus==1) && (*Bedroom2LightStatus==0) && (*Bedroom1ButtonPressed==1))
{
	*Bedroom2LightStatus=1;
	pSetTimer(TIMER_LONGCLICK,-1);
}
if(pGetTimer(TIMER_LONGCLICK)==0 && (*Bedroom1LightStatus==0) && (*Bedroom2LightStatus==1) && (*Bedroom1ButtonPressed==1))
{
	*Bedroom2LightStatus=0;
	pSetTimer(TIMER_LONGCLICK,-1);
}

// Bedroom 3 (bed right side)

if((pGetNodeVar(DEV_SPIDER,IN4)==1) && *Bedroom3LightStatus==0 && (*Bedroom3ButtonPressed==0))
{
	*Bedroom3LightStatus=1;
	*Bedroom3ButtonPressed=1;
	pSetTimer(TIMER_LONGCLICK,LONGCLICK_TIME);
}
if((pGetNodeVar(DEV_SPIDER,IN4)==1) && *Bedroom3LightStatus==1 && (*Bedroom3ButtonPressed==0))
{
	*Bedroom3LightStatus=0;
	*Bedroom3ButtonPressed=1;
	pSetTimer(TIMER_LONGCLICK,LONGCLICK_TIME);
}


if(pGetTimer(TIMER_LONGCLICK)==0 && (*Bedroom1LightStatus==1) && (*Bedroom3LightStatus==0) && (*Bedroom1ButtonPressed==1))
{
	*Bedroom3LightStatus=1;
	pSetTimer(TIMER_LONGCLICK,-1);
}
if(pGetTimer(TIMER_LONGCLICK)==0 && (*Bedroom1LightStatus==0) && (*Bedroom3LightStatus==1) && (*Bedroom1ButtonPressed==1))
{
	*Bedroom3LightStatus=0;
	pSetTimer(TIMER_LONGCLICK,-1);
}


// == Switches click (end) ==


// == Switches click reset (beginning) ==

if((pGetNodeVar(DEV_SPIDER,IN1)==0))
{
	*Livingroom1ButtonPressed=0;
}

if((pGetNodeVar(DEV_SPIDER,IN1)==0))
{
	*Livingroom2ButtonPressed=0;
}

if((pGetNodeVar(DEV_SPIDER,IN2)==0))
{
	*Kitchen1ButtonPressed=0;
}

if((pGetNodeVar(DEV_SPIDER,IN2)==0))
{
	*Kitchen2ButtonPressed=0;
}

if((pGetNodeVar(DEV_IN1,IN4)==0))
{
	*ClosetButtonPressed=0;
}

if((pGetNodeVar(DEV_SPIDER,IN3)==0))
{
	*CorridorButtonPressed=0;
}

if((pGetNodeVar(DEV_RELAY1,IN1)==0))
{
	*BathroomButtonPressed=0;
}

if((pGetNodeVar(DEV_RELAY1,IN2)==0))
{
	*StudyButtonPressed=0;
}

if((pGetNodeVar(DEV_RELAY1,IN3)==0))
{
	*Bedroom1ButtonPressed=0;
}

if((pGetNodeVar(DEV_SPIDER,IN4)==0))
{
	*Bedroom2ButtonPressed=0;
}

if((pGetNodeVar(DEV_SPIDER,IN4)==0))
{
	*Bedroom3ButtonPressed=0;
}

// == Switches click reset (end) ==

// == Light control with light sensor (beginning) ==

if(pGetTimer(TIMER_LIGHT_LIVINGROOM)==-1)
{
	*LivingroomLightAvg=*LivingroomLightCount>0 ? *LivingroomLightSum/(*LivingroomLightCount)*0.0234375 : 0;
	*LivingroomLightSum=0;
	*LivingroomLightCount=0;
	pSetTimer(TIMER_LIGHT_LIVINGROOM,6000);

}
else
{
	*LivingroomLightSum+=pGetNodeVar(DEV_SPIDER,AIN1);
	(*LivingroomLightCount)++;
}

if(pTimeCmp(0xFF,0xFF,0xFF,0,0,0,0xFF,1)==0)
	*Livingroom1LightSensorWorked=0;

if(pTimeCmp(0xFF,0xFF,0xFF,16,0,0,0xFF,3)==0 && *GuardStatus==0 && *Livingroom1LightSensorWorked==0 && *LivingroomLightAvg<0.02)
{
	*Livingroom1LightSensorWorked=1;
	*Livingroom1LightStatus=1;
}

// == Light control with light sensor (end) ==


// == Light control with motion detector (beginning) ==

// Bathroom

if((pGetNodeVar(DEV_RELAY2,IN2)==0))
{
	pSetTimer(TIMER_MOVE_BATHROOM,4000);
}
if((pGetNodeVar(DEV_RELAY2,IN2)==0) && (*BathroomLightStatusMove==0) && (*BathroomLightStatusMoveEnabled==1))
{
	*BathroomLightStatusMove=1;
}
if((*BathroomLightStatusMove==1) && ( (pGetTimer(TIMER_MOVE_BATHROOM)==0) || (*BathroomLightStatusMoveEnabled==0) ))
{
	*BathroomLightStatusMove=0;
}

// Corridor
if((pGetNodeVar(DEV_RELAY2,IN3)==0))
{
	pSetTimer(TIMER_MOVE_CORRIDOR,3000);
	*CorridorMoveDetected=1;
}
if((pGetNodeVar(DEV_RELAY2,IN3)==0) && (*CorridorLightStatusMove==0) && (*CorridorLightStatusMoveEnabled==1))
{
	*CorridorLightStatusMove=1;
}
if((*CorridorLightStatusMove==1) && ( (pGetTimer(TIMER_MOVE_CORRIDOR)==0) || (*CorridorLightStatusMoveEnabled==0)  ))
{
	*CorridorLightStatusMove=0;
}



// == Light control with motion detector (end) ==

// == Alarm ==

if( (KeyIgor==1 || KeyOlga==1 || KeyReserve==1) && *GuardStatus==0 && (pGetTimer(TIMER_GUARD)==-1))
{
	*GuardStatus=1;
}

if(*GuardStatus==1)
{
	if(pGetNodeVar(DEV_RELAY2,IN1)==1 && pGetNodeVar(DEV_RELAY2,IN2)==1 && pGetNodeVar(DEV_RELAY3,IN1)==1 && pGetNodeVar(DEV_RELAY3,IN2)==1)
	{
		pSetTimer(TIMER_MOVE_BATHROOM,-1);
		*Livingroom1LightStatus=0;
		*Livingroom2LightStatus=0;
		*Kitchen1LightStatus=0;
		*Kitchen2LightStatus=0;
		*ClosetLightStatus=0;
		*BathroomLightStatus=0;
		*BathroomLightStatusMove=0;
		*BathroomVentStatus=0;
		*StudyLightStatus=0;
		*Bedroom1LightStatus=0;
		*Bedroom2LightStatus=0;
		*Bedroom3LightStatus=0;

		*GuardStatus=2;
		pSetTimer(TIMER_GUARD,3000); // 30 seconds to leave
		pSetTimer(TIMER_GUARD_BEEP_UP, 100);

		put2log[0]=2;
		if(KeyIgor==1)
			put2log[1]=1;
		else if(KeyOlga==1)
			put2log[1]=2;
		else if(KeyReserve==1)
			put2log[1]=3;
		else
			put2log[1]=0;

	}
	else
	{
		*GuardStatus=0;
		pSetTimer(TIMER_GUARD,300);
		pSetTimer(TIMER_GUARD_BEEP_ERROR, 160);

		put2log[0]=1;
		if(pGetNodeVar(DEV_RELAY2,IN1)==0)
			put2log[1]=1;
		if(pGetNodeVar(DEV_RELAY2,IN2)==0)
			put2log[1]=2;
		if(pGetNodeVar(DEV_RELAY3,IN1)==0)
			put2log[1]=3;
		if(pGetNodeVar(DEV_RELAY3,IN2)==0)
			put2log[1]=4;
	}
}

// Alarm switching ON buzzer
if(pGetTimer(TIMER_GUARD_BEEP_UP)==100)
	*BuzzerStatus=1;
if(pGetTimer(TIMER_GUARD_BEEP_UP)==80)
	*BuzzerStatus=0;
if(pGetTimer(TIMER_GUARD_BEEP_UP)==60)
	*BuzzerStatus=1;
if(pGetTimer(TIMER_GUARD_BEEP_UP)==40)
	*BuzzerStatus=0;
if(pGetTimer(TIMER_GUARD_BEEP_UP)==20)
	*BuzzerStatus=1;
if(pGetTimer(TIMER_GUARD_BEEP_UP)==0)
	*BuzzerStatus=0;
// Alarm switching ON buzzer (end)

// Alarm switching ON error buzzer
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==160)
	*BuzzerStatus=1;
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==150)
	*BuzzerStatus=0;
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==130)
	*BuzzerStatus=1;
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==90)
	*BuzzerStatus=0;
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==70)
	*BuzzerStatus=1;
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==60)
	*BuzzerStatus=0;
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==40)
	*BuzzerStatus=1;
if(pGetTimer(TIMER_GUARD_BEEP_ERROR)==0)
	*BuzzerStatus=0;
// Alarm switching ON error buzzer (end)

if((*GuardStatus==2) && (pGetTimer(TIMER_GUARD)==-1))
{
	pSetTimer(TIMER_MOVE_CORRIDOR,-1);
	*CorridorLightStatus=0;
	*CorridorLightStatusMove=0;
	*GuardStatus=3;
	pSetTimer(TIMER_GUARD_BEEP_ERROR, 50);

	if(lastlog[0]==2 && lastlog[1]==1)
		pSendSMS(PHONE1, "041F043E044104420430043D043E0432043A04300020043D04300020043E044504400430043D0443", NULL, NULL); // Send ok message
	else if(lastlog[0]==2 && lastlog[1]==2)
		pSendSMS(PHONE2, "041F043E044104420430043D043E0432043A04300020043D04300020043E044504400430043D0443", NULL, NULL);
	else if(lastlog[0]==2)
		pSendSMS(PHONE1, "041F043E044104420430043D043E0432043A04300020043D04300020043E044504400430043D0443", NULL, NULL);

}

if( (KeyIgor==1 || KeyOlga==1 || KeyReserve==1) && *GuardStatus>=3)
{
	*GuardStatus=0;
	*BuzzerStatus=0;
	pSetTimer(TIMER_GUARD,300);
	pSetTimer(TIMER_GUARD_BEEP_UP, 110);
	put2log[0]=3;

	if(KeyIgor==1)
		put2log[1]=1;
	else if(KeyOlga==1)
		put2log[1]=2;
	else if(KeyReserve==1)
		put2log[1]=3;
	else
		put2log[1]=0;
}

if((pGetNodeVar(DEV_RELAY2,IN1)==0 || pGetNodeVar(DEV_RELAY2,IN2)==0 || pGetNodeVar(DEV_RELAY2,IN3)==0 || pGetNodeVar(DEV_RELAY3,IN1)==0 || pGetNodeVar(DEV_RELAY3,IN2)==0)  && pGetTimer(TIMER_GUARD)==-1 && *GuardStatus==3)
{
	*GuardStatus=4;
	pSetTimer(TIMER_GUARD,1000);

	put2log[0]=5;

	if(pGetNodeVar(DEV_RELAY2,IN1)==0) // Living room
		put2log[1]=1;
	if(pGetNodeVar(DEV_RELAY2,IN2)==0) // Bathroom
		put2log[1]=2;
	if(pGetNodeVar(DEV_RELAY2,IN3)==0) // Corridor
		put2log[1]=3;
	if(pGetNodeVar(DEV_RELAY3,IN1)==0) // Study room
		put2log[1]=4;
	if(pGetNodeVar(DEV_RELAY3,IN2)==0) // Bedroom
		put2log[1]=5;
}

if(pGetNodeVar(DEV_IN1,IN1)==1 || pGetNodeVar(DEV_IN1,IN2)==1 || pGetNodeVar(DEV_IN1,IN3)==1)
{
	*GuardStatus=4;
	pSetTimer(TIMER_GUARD,1000);

	put2log[0]=5;
	if(pGetNodeVar(DEV_IN1,IN2)==1) // Water leak under the bath
		put2log[1]=6;
	if(pGetNodeVar(DEV_IN1,IN1)==1) // Water leak near the pipes
		put2log[1]=7;
	if(pGetNodeVar(DEV_IN1,IN3)==1) // Water leak in the kitchen
		put2log[1]=8;
}

if((pGetTimer(TIMER_GUARD)==-1) && (*GuardStatus==4))
{
	*GuardStatus=5;
	put2log[0]=4;

	if(pGetNodeVar(DEV_RELAY2,IN1)==0) // Living room
		put2log[1]=1;
	else if(pGetNodeVar(DEV_RELAY2,IN2)==0) // Bathroom
		put2log[1]=2;
	else if(pGetNodeVar(DEV_RELAY2,IN3)==0) // Corridor
		put2log[1]=3;
	else if(pGetNodeVar(DEV_RELAY3,IN1)==0) // Study room
		put2log[1]=4;
	else if(pGetNodeVar(DEV_RELAY3,IN2)==0) // Bedroom
		put2log[1]=5;
	else if(pGetNodeVar(DEV_IN1,IN2)==1) // Water leak under the bath
		put2log[1]=6;
	else if(pGetNodeVar(DEV_IN1,IN1)==1) // Water leak near the pipes
		put2log[1]=7;
	else if(pGetNodeVar(DEV_IN1,IN3)==1) // Water leak in the kitchen
		put2log[1]=8;
	else if(lastlog[0]==5)
		put2log[1]=lastlog[1];
}

if((pGetTimer(TIMER_GUARD)==-1) && (*GuardStatus==5))
{
	pSetTimer(TIMER_GUARD,120000);
}

if((pGetTimer(TIMER_GUARD)==119999) && (*GuardStatus==5))
{
	if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==1) // ?????????
	{
		pSendSMS(PHONE1, "0422044004350432043E043304300021002004140432043804360435043D043804350020043200200433043E044104420438043D043D043E0439", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==2) // ??????
	{
		pSendSMS(PHONE1, "0422044004350432043E043304300021002004140432043804360435043D0438043500200432002004320430043D043D043E0439", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==3) // ???????
	{
		pSendSMS(PHONE1, "0422044004350432043E043304300021002004140432043804360435043D04380435002004320020043A043E044004380434043E04400435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==4) // ???????
	{
		pSendSMS(PHONE1, "0422044004350432043E043304300021002004140432043804360435043D04380435002004320020043A043004310438043D043504420435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==5) // ???????
	{
		pSendSMS(PHONE1, "0422044004350432043E043304300021002004140432043804360435043D043804350020043200200441043F0430043B044C043D0435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==6) // ???????? ?????? ??? ??????
	{
		pSendSMS(PHONE1, "0422044004350432043E0433043000210020041F0440043E044204350447043A043000200432002004320430043D043D043E04390020043F043E0434002004320430043D043D043E0439", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==7) // ???????? ?????? ?????
	{
		pSendSMS(PHONE1, "0422044004350432043E0433043000210020041F0440043E044204350447043A043000200432002004320430043D043D043E04390020043D0430002004410442043E044F043A0435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==8) // ???????? ?????
	{
		pSendSMS(PHONE1, "0422044004350432043E0433043000210020041F0440043E044204350447043A04300020043D04300020043A04430445043D0435", NULL, NULL);
	}
	else if(lastlog[0]==4 || lastlog[0]==5)
	{
		pSendSMS(PHONE1, "0422044004350432043E043304300021", NULL, NULL);
	}
}

if((pGetTimer(TIMER_GUARD)==116999) && (*GuardStatus==5))
{
	if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==1) // ?????????
	{
		pSendSMS(PHONE2, "0422044004350432043E043304300021002004140432043804360435043D043804350020043200200433043E044104420438043D043D043E0439", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==2) // ??????
	{
		pSendSMS(PHONE2, "0422044004350432043E043304300021002004140432043804360435043D0438043500200432002004320430043D043D043E0439", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==3) // ???????
	{
		pSendSMS(PHONE2, "0422044004350432043E043304300021002004140432043804360435043D04380435002004320020043A043E044004380434043E04400435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==4) // ???????
	{
		pSendSMS(PHONE2, "0422044004350432043E043304300021002004140432043804360435043D04380435002004320020043A043004310438043D043504420435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==5) // ???????
	{
		pSendSMS(PHONE2, "0422044004350432043E043304300021002004140432043804360435043D043804350020043200200441043F0430043B044C043D0435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==6) // ???????? ?????? ??? ??????
	{
		pSendSMS(PHONE2, "0422044004350432043E0433043000210020041F0440043E044204350447043A043000200432002004320430043D043D043E04390020043F043E0434002004320430043D043D043E0439", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==7) // ???????? ?????? ?????
	{
		pSendSMS(PHONE2, "0422044004350432043E0433043000210020041F0440043E044204350447043A043000200432002004320430043D043D043E04390020043D0430002004410442043E044F043A0435", NULL, NULL);
	}
	else if((lastlog[0]==4 || lastlog[0]==5) && lastlog[1]==8) // ???????? ?????
	{
		pSendSMS(PHONE2, "0422044004350432043E0433043000210020041F0440043E044204350447043A04300020043D04300020043A04430445043D0435", NULL, NULL);
	}
	else if(lastlog[0]==4 || lastlog[0]==5)
	{
		pSendSMS(PHONE2, "0422044004350432043E043304300021", NULL, NULL);
	}
}


// Alarm switching OFF buzzer (beginning)
if(*GuardStatus==4)
{
	if(pGetTimer(TIMER_GUARD)%100==0)
		*BuzzerStatus=1;
	if((pGetTimer(TIMER_GUARD)-50)%100==0)
		*BuzzerStatus=0;
}
// Alarm switching OFF buzzer (end)


// Alarm (beginning)
if(*GuardStatus==5 && pGetTimer(TIMER_GUARD)>114000)
{
	if(pGetTimer(TIMER_GUARD)%300==0)
		*BuzzerStatus=1;
	if((pGetTimer(TIMER_GUARD)-100)%300==0)
		*BuzzerStatus=0;
}
else if(*GuardStatus==5 && *BuzzerStatus==1)
{
	*BuzzerStatus=0;
}
// Alarm (end)


// == Alarm (end) ==

// == Water leak sensors (beginning) ==

if(pGetTimer(TIMER_WATER_BATHROOM1)==-1)
{
	*BathroomWaterAvg1=*BathroomWaterCount1>0 ? *BathroomWaterSum1/(*BathroomWaterCount1)*0.0234375 : 0;
	*BathroomWaterSum1=0;
	*BathroomWaterCount1=0;
	pSetTimer(TIMER_WATER_BATHROOM1,6000);

}
else
{
	*BathroomWaterSum1+=pGetNodeVar(DEV_RELAY1,AIN2);
	(*BathroomWaterCount1)++;

}

if(pGetTimer(TIMER_WATER_BATHROOM2)==-1)
{
	*BathroomWaterAvg2=*BathroomWaterCount2>0 ? *BathroomWaterSum2/(*BathroomWaterCount2)*0.0234375 : 0;
	*BathroomWaterSum2=0;
	*BathroomWaterCount2=0;
	pSetTimer(TIMER_WATER_BATHROOM2,6000);

}
else
{
	*BathroomWaterSum2+=pGetNodeVar(DEV_RELAY1,AIN3);
	(*BathroomWaterCount2)++;
}

if(pGetTimer(TIMER_WATER_KITCHEN)==-1)
{
	*KitchenWaterAvg=*KitchenWaterCount>0 ? *KitchenWaterSum/(*KitchenWaterCount)*0.0234375 : 0;
	*KitchenWaterSum=0;
	*KitchenWaterCount=0;
	pSetTimer(TIMER_WATER_KITCHEN,6000);

}
else
{
	*KitchenWaterSum+=pGetNodeVar(DEV_RELAY2,AIN1);
	(*KitchenWaterCount)++;
}

// == Water leak sensors (end) ==


// == Underfloor heating (beginning) ==

// Bathroom

*BathroomFloorTempCurrent=pGetNodeVar(DEV_TEMP1,AIN1)*0.01-327.67;

if(*BathroomFloorTempCurrent<=0 && pGetTimer(TIMER_INIT)==0)
	*BathroomFloorEnabled=0;

if(*BathroomFloorTempExpected<10 || *BathroomFloorTempExpected>40)
{
	*BathroomFloorTempExpected=DEFAULT_FLOOR_TEMP_BATHROOM;
}

if(*BathroomFloorTempCurrent < *BathroomFloorTempExpected-FLOOR_TEMP_DELTA && *BathroomFloorEnabled==1)
{
	*BathroomFloorStatus=1;
}
if(*BathroomFloorTempCurrent > *BathroomFloorTempExpected+FLOOR_TEMP_DELTA && *BathroomFloorEnabled==1)
{
	*BathroomFloorStatus=0;
}
if(*BathroomFloorEnabled==0)
{
	*BathroomFloorStatus=0;
}

// Kitchen

*KitchenFloorTempCurrent=pGetNodeVar(DEV_TEMP2,AIN1)*0.01-327.67;

if(*KitchenFloorTempCurrent<10 && pGetTimer(TIMER_INIT)==0)
	*KitchenFloorEnabled=0;

if(*KitchenFloorTempExpected<10 || *KitchenFloorTempExpected>40)
{
	*KitchenFloorTempExpected=DEFAULT_FLOOR_TEMP_KITCHEN;
}

if(*KitchenFloorTempCurrent < *KitchenFloorTempExpected-FLOOR_TEMP_DELTA && *KitchenFloorEnabled==1)
{
	*KitchenFloorStatus=1;
}
if(*KitchenFloorTempCurrent > *KitchenFloorTempExpected+FLOOR_TEMP_DELTA && *KitchenFloorEnabled==1)
{
	*KitchenFloorStatus=0;
}
if(*KitchenFloorEnabled==0)
{
	*KitchenFloorStatus=0;
}


// == Underfloor heating (end) ==

// == Lighting (beginning) ==

if(*Livingroom1LightStatus==1)
{
	if(pGetNodeVar(DEV_SPIDER,OUT1)==0)
	{
		pSetNodeVar(DEV_SPIDER,OUT1,1);
	}

}
else if(*Livingroom1LightStatus==0)
{
	if(pGetNodeVar(DEV_SPIDER,OUT1)==1)
		pSetNodeVar(DEV_SPIDER,OUT1,0);
}

if(*Kitchen1LightStatus==1)
{
	if(pGetNodeVar(DEV_SPIDER,OUT2)==0)
		pSetNodeVar(DEV_SPIDER,OUT2,1);
}
else if(*Kitchen1LightStatus==0)
{
	if(pGetNodeVar(DEV_SPIDER,OUT2)==1)
		pSetNodeVar(DEV_SPIDER,OUT2,0);
}

if(*ClosetLightStatus==1)
{
	if(pGetNodeVar(DEV_RELAY2,OUT2)==0)
		pSetNodeVar(DEV_RELAY2,OUT2,1);
}
else if(*ClosetLightStatus==0)
{
	if(pGetNodeVar(DEV_RELAY2,OUT2)==1)
		pSetNodeVar(DEV_RELAY2,OUT2,0);
}


if(*CorridorLightStatus==1 || *CorridorLightStatusMove==1)
{
	if(
		(*CorridorLightStatus==1 || pTimeCmp(0xFF,0xFF,0xFF,18,0,0,0xFF,3)==0 && pTimeCmp(0xFF,0xFF,0xFF,23,30,0,0xFF,4)==0 || *GuardStatus>3)
		&& pGetNodeVar(DEV_SPIDER,OUT4)==0)
		pSetNodeVar(DEV_SPIDER,OUT4,1);
	if(
		(*CorridorLightStatus==1 || pTimeCmp(0xFF,0xFF,0xFF,18,0,0,0xFF,3)==0 && pTimeCmp(0xFF,0xFF,0xFF,23,30,0,0xFF,4)==0 || pTimeCmp(0xFF,0xFF,0xFF,23,30,0,0xFF,3)==0 || pTimeCmp(0xFF,0xFF,0xFF,7,0,0,0xFF,4)==0 || *GuardStatus>3)
		&& pGetNodeVar(DEV_RELAY1,OUT3)==0)
		pSetNodeVar(DEV_RELAY1,OUT3,1);
}
else if(*CorridorLightStatus==0 && *CorridorLightStatusMove==0)
{
	if(pGetNodeVar(DEV_SPIDER,OUT4)==1)
		pSetNodeVar(DEV_SPIDER,OUT4,0);
	if(pGetNodeVar(DEV_RELAY1,OUT3)==1)
		pSetNodeVar(DEV_RELAY1,OUT3,0);
}

if(*BathroomLightStatus==1 || *BathroomLightStatusMove==1)
{
	if(
		(*BathroomLightStatus==1 || pTimeCmp(0xFF,0xFF,0xFF,7,0,0,0xFF,3)==0)
		&& pGetNodeVar(DEV_RELAY2,OUT1)==0)
		pSetNodeVar(DEV_RELAY2,OUT1,1);
	if(pGetNodeVar(DEV_RELAY1,OUT1)==0)
		pSetNodeVar(DEV_RELAY1,OUT1,1);
}
else if(*BathroomLightStatus==0 && *BathroomLightStatusMove==0)
{
	if(pGetNodeVar(DEV_RELAY2,OUT1)==1)
		pSetNodeVar(DEV_RELAY2,OUT1,0);
	if(pGetNodeVar(DEV_RELAY1,OUT1)==1)
		pSetNodeVar(DEV_RELAY1,OUT1,0);
}

if(*StudyLightStatus==1)
{
	if(pGetNodeVar(DEV_RELAY3,OUT1)==0)
		pSetNodeVar(DEV_RELAY3,OUT1,1);
}
else if(*StudyLightStatus==0)
{
	if(pGetNodeVar(DEV_RELAY3,OUT1)==1)
		pSetNodeVar(DEV_RELAY3,OUT1,0);
}

if(*Bedroom1LightStatus==1)
{
	if(pGetNodeVar(DEV_RELAY2,OUT4)==0)
		pSetNodeVar(DEV_RELAY2,OUT4,1);
}
else if(*Bedroom1LightStatus==0)
{
	if(pGetNodeVar(DEV_RELAY2,OUT4)==1)
		pSetNodeVar(DEV_RELAY2,OUT4,0);
}

if(*Bedroom2LightStatus==1)
{
	if(pGetNodeVar(DEV_RELAY1,OUT2)==0)
		pSetNodeVar(DEV_RELAY1,OUT2,1);
}
else if(*Bedroom2LightStatus==0)
{
	if(pGetNodeVar(DEV_RELAY1,OUT2)==1)
		pSetNodeVar(DEV_RELAY1,OUT2,0);
}

if(*BuzzerStatus==1)
{
	if(pGetNodeVar(DEV_RELAY1,OUT4)==0)
		pSetNodeVar(DEV_RELAY1,OUT4,1);
}
else if(*BuzzerStatus==0)
{
	if(pGetNodeVar(DEV_RELAY1,OUT4)==1)
		pSetNodeVar(DEV_RELAY1,OUT4,0);
}


if(*BathroomFloorStatus==1)
{
	if(pGetNodeVar(DEV_RELAY2,OUT3)==0)
		pSetNodeVar(DEV_RELAY2,OUT3,1);
}
else if(*BathroomFloorStatus==0)
{
	if(pGetNodeVar(DEV_RELAY2,OUT3)==1)
		pSetNodeVar(DEV_RELAY2,OUT3,0);
}

if(*KitchenFloorStatus==1)
{
	if(pGetNodeVar(DEV_SPIDER,OUT3)==0)
		pSetNodeVar(DEV_SPIDER,OUT3,1);
}
else if(*KitchenFloorStatus==0)
{
	if(pGetNodeVar(DEV_SPIDER,OUT3)==1)
		pSetNodeVar(DEV_SPIDER,OUT3,0);
}


// == Lighting (end) ==

/*
Logging

Event types:
1 - Unsuccessful alarm switching ON
2 - Successful alarm switching ON
3 - Alarm switching OFF
4 - Alarm

Alarm sensors types:
1 - Motion
100 - Water leak

Keys code
1 - Igor
2 - Olga
3 - Reserve

*/

if(put2log[0]>0)
{
	if(*LogCounter>=20)
	{
		*LogCounter=0;
	}

	pInts[LOG_START_INDEX+1+*LogCounter*4]=put2log[0];
	pInts[LOG_START_INDEX+1+*LogCounter*4+1]=put2log[1];
	pInts[LOG_START_INDEX+1+*LogCounter*4+2]=put2log[2];
	pInts[LOG_START_INDEX+1+*LogCounter*4+3]=put2log[3];
	*LogCounter=*LogCounter+1;
}

if((*GlobalInit)==0)
{
	*GlobalInit=1;
}

}