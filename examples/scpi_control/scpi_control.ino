/*
AUTHOR: Micah Black, A2D Electronics
DATE: Jan 24, 2021
PURPOSE: This example implements some SCPI commands
        (that don't completely follow the susbystem style standard)
        to communicate with the DAQ.
CHANGELOG:
	Feb 18, 2021 - Added Serial.flush(); after serial println commands on query
*/

#include <A2D_DAQ.h>

#define MANUFACTURER  ("A2D Electronics")
#define DESCRIPTION ("64 Channel DAQ")
#define VERSION     ("V1.0.0")

//SERIAL DEFINES
#define BAUDRATE    57600
#define SER_BUF_LEN   32
#define END_CHAR    '\n'
#define NO_CMD      ""

//Macro for finding commands - F to store string literal
//in flash instead of memory
#define CMDIS(i,c) (!strcmp_P(i, PSTR(c)))

//Function Prototypes:
void parse_serial(char ser_buf[], char command[], uint8_t* channel_num, bool* value_bool);

A2D_DAQ daq;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
  daq.A2D_DAQ_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Allocate memory for the serial buffer
  char ser_buf[SER_BUF_LEN];
  char command[SER_BUF_LEN];
  uint8_t chars_input = 0;
  uint8_t channel_num = 0;
  bool value_bool = 0;
  
  //if serial data is available
  if(Serial.available()){
    //Read until a full command is received
    chars_input = Serial.readBytesUntil(END_CHAR, ser_buf, SER_BUF_LEN);
    //terminate the input string with NULL
    ser_buf[chars_input] = '\0'; 
    //if(chars_input == 0);//TODO - set a default command to read
    //}
	  parse_serial(ser_buf, command, &channel_num, &value_bool);
  }
  else{
    strcpy(command, "NOCMD");
  }


  //NOCMD?
  if(CMDIS(command, "NOCMD")){
    ;
  }
  
  //*IDN?
  else if(CMDIS(command, "*IDN?")){
    Serial.print(MANUFACTURER);
    Serial.print(" ");
    Serial.print(DESCRIPTION);
    Serial.print(" ");
    Serial.println(VERSION);
    Serial.flush();
  }
  
  //*RST
  else if (CMDIS(command, "*RST")){
    daq.A2D_DAQ_reset();
  }
  
  //*CLS
  else if (CMDIS(command, "*CLS")){
    ; //nothing since we don't have errors yet
  }
  
  //CONF:DAQ:OUTP (@ch) //just work with 1 ch for now
  else if (CMDIS(command, "CONF:DAQ:OUTP")){
    A2D_DAQ_channel_config conf = daq.A2D_DAQ_get_default_config();
    conf.channel_dir = A2D_DAQ_OUTPUT;
    daq.A2D_DAQ_config_channel(channel_num, conf);
  }
  
  //CONF:DAQ:INP (@ch) //configure all channels as input
  else if (CMDIS(command, "CONF:DAQ:INP")){
    A2D_DAQ_channel_config conf = daq.A2D_DAQ_get_default_config();
    conf.channel_dir = A2D_DAQ_INPUT;
    daq.A2D_DAQ_config_channel(channel_num, conf);
  }
  
  //INSTR:DAQ:SET:OUTP (@ch),VAL //VAL is boolean 0 or 1
  else if (CMDIS(command, "INSTR:DAQ:SET:OUTP")){
    daq.A2D_DAQ_set_dig_out(channel_num, value_bool);
  }
  
  //INSTR:READ:ANA? (@ch) //read ADC ch_list and return
  else if (CMDIS(command, "INSTR:READ:ANA?")){
    Serial.println(daq.A2D_DAQ_get_analog_mv(channel_num));
    Serial.flush();
  }
  
  //INSTR:READ:DIG? (@ch) //read digital input register and return
  else if (CMDIS(command, "INSTR:READ:DIG?")){
    Serial.println(daq.A2D_DAQ_get_dig_in(channel_num));
    Serial.flush();
  }
}

void parse_serial(char ser_buf[], char command[], uint8_t *channel_num, bool *value_bool){
  //All SCPI commands are terminated with newline '/n'
  //but the Serial.readBytesUntil discards the terminator
  //so do we need to add one to use strcmp?
  
  //we will assume only 1 command is sent at a time
  //so we don't have to deal with SCPI's ';' to send
  //multiple commands on the same line
  
  //split input string on space to extract the command
  //and the parameters
  //strtok replaces the delimeter with NULL to terminate
  //the string
  char delimeters[] = " ,";
  char* token;
  char channel_str[8];
  char value_str[8];
  
  token = strtok(ser_buf, delimeters);
  strcpy(command, token);
  token = strtok(NULL, delimeters);
  strcpy(channel_str, token);
  token = strtok(NULL, delimeters);
  strcpy(value_str, token);

  token = strtok(value_str, delimeters);
  *value_bool = atoi(token);
  if(*value_bool > 1)
    *value_bool = 1;
  else if(*value_bool < 0)
    *value_bool = 0;
  
  strcpy(delimeters, "@)");
  token = strtok(channel_str, delimeters); //get rid of "(@"
  token = strtok(NULL, delimeters);
  *channel_num = atoi(token);
  if(*channel_num > 63)
    *channel_num = 63;
  else if(*channel_num < 0)
    *channel_num = 0;

  /*
  Serial.print(F("Serial Buffer:"));
  Serial.println(ser_buf);
  Serial.print(F("Command:"));
  Serial.println(command);
  Serial.print(F("Channel:"));
  Serial.println(*channel_num);
  Serial.print(F("Value:"));
  Serial.println(*value_bool);
  Serial.println("");
  */

}