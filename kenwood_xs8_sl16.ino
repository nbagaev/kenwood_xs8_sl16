/*
Kenwood XS/XS8/SL16 remote control
tested on KXF-W4010
pinout: sleeve = GND ring = DATA tip = BUSY

           R 
pin3 o---[   ]----o tip
          330
          
           R 
pin2 o---[   ]----o ring
          330
*/

//arduino pins
const uint8_t  DATA = 2;
const uint8_t  BUSY = 3;
const uint8_t  RESPONSE = 4;//for loop stopping at event like motor start

uint16_t last_cmd = 1;
uint8_t  interface = 8;

//timing XS8
const uint8_t START_BIT_L_8 = 1;
const uint8_t START_BIT_H_8 = 10;
const uint8_t BIT_0L_8 = 10;
const uint8_t BIT_1L_8 = 5;
const uint8_t BIT_H_8 = 5;

//timing SL16
const uint8_t START_BIT_L_16 = 5;
const uint8_t START_BIT_H_16 = 5;
const uint8_t BIT_0L_16 = 5;
const uint8_t BIT_1L_16 = 2;
const uint8_t BIT_H_16 = 2;

//delays
uint8_t START_BIT_L;
uint8_t START_BIT_H;
uint8_t BIT_0L;
uint8_t BIT_1L;
uint8_t BIT_H;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  pinMode(RESPONSE, INPUT);
  pinMode(BUSY, INPUT);
  pinMode(DATA, INPUT);
  set_interface(-8);

  // Usage

  Serial.println("Type:");

  Serial.println("  value -8 to set interface to XS/XS8. Default is XS/XS8");
  Serial.println("  value -16 to set interface to SL16");
  Serial.println("  value >0 to send command. It will be saved as 'last command'");
  Serial.println("  value <-16 to start a loop to automatically try all commands with delay of |value| ms beginning at 'last command'. Default 'last command' = 1");
}

void set_interface(int i)
{
  interface = -i;
  Serial.print("set interface to ");
  Serial.println(interface, DEC);
  if(interface == 8)
  {
    START_BIT_L = START_BIT_L_8;
    START_BIT_H = START_BIT_H_8;
    BIT_0L = BIT_0L_8;
    BIT_1L = BIT_1L_8;
    BIT_H = BIT_H_8;
  }
  else
  {
    START_BIT_L = START_BIT_L_16;
    START_BIT_H = START_BIT_H_16;
    BIT_0L = BIT_0L_16;
    BIT_1L = BIT_1L_16;
    BIT_H = BIT_H_16;
  }
}

void send_cmd(uint16_t cmd)
{
  Serial.print("Interface ");
  Serial.print(interface, DEC);
  Serial.print(" Command ");
  Serial.print(cmd, BIN);
  Serial.print(" / 0x");
  Serial.println(cmd, HEX);

  //test busy
  if(!digitalRead(BUSY))
  {
    pinMode(BUSY, OUTPUT);
    pinMode(DATA, OUTPUT);
    digitalWrite(BUSY, LOW);
    digitalWrite(DATA, LOW);
    //BUSY on
    digitalWrite(BUSY, HIGH);
    //start bit
    delay(START_BIT_L);
    digitalWrite(DATA, HIGH);
    delay(START_BIT_H);
  
    //data bits
    for (uint16_t mask = 1U << (interface - 1); mask; mask >>= 1)
    {    
      digitalWrite(DATA, LOW);
      if (cmd & mask)
        delay(BIT_1L);                                 
      else
        delay(BIT_0L);
      digitalWrite(DATA, HIGH);
      delay(BIT_H);    
    }

    //end command 
    digitalWrite(DATA, LOW);
    //BUSY off
    digitalWrite(BUSY, LOW);
    delay(1);
    //go standby    
    pinMode(BUSY, INPUT);
    pinMode(DATA, INPUT);
    last_cmd = cmd;
    }
}

void try_all(int wait)
{
  for (uint16_t cmd = last_cmd; cmd < 1U << (interface - 1); cmd++)
  {    
    send_cmd(cmd);
    delay(-wait);
    if(digitalRead(RESPONSE))
      break;
  }
}

void loop()
{
  while (Serial.available())
  {
    long val = Serial.parseInt();
    if (val == -8 || val == -16)
      set_interface(val);
    else if(val > 0)
      send_cmd(val);
    else if(val < -16)      
      try_all(val);     
  }
}
