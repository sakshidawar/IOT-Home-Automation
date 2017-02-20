
#define DST_IP "myiotprojects.com" // The IP where the PHP Site is hosted.

//Defined the pin for the on board LED
int led = 8;

//Renaming the Pins of the microprocessor.
#define R1 A0
#define R2 A1
#define R3 A2
#define R4 A3


//initiating the variables to be used
String iot_data,str;
int counter = 0;
int lastIndex = 0;
const int numberOfPieces = 4;
String pieces[numberOfPieces];

int len=0;
int i=0;
char *buff, *token;



#include <EEPROM.h>
//included the eprom header file to use its methods.

//Initialize Device
void relay_action()
{
//_________________________Relay control_______________________________

//reading the signals from the memory and sending it to the microprocessor's pins.
   if(EEPROM.read(11)==1)
   {
    digitalWrite(R1,HIGH);
	
   }
  else if(EEPROM.read(11)==0)
   {
    digitalWrite(R1,LOW);
	
   }

   if(EEPROM.read(12)==1)
   {
    digitalWrite(R2,HIGH);
	
   }
  else if(EEPROM.read(12)==0)
   {
    digitalWrite(R2,LOW);
	
   }

   if(EEPROM.read(13)==1)
   {
    digitalWrite(R3,HIGH);
	
   }
  else if(EEPROM.read(13)==0)
   {
    digitalWrite(R3,LOW);
	
   }

   if(EEPROM.read(14)==1)
   {
    digitalWrite(R4,HIGH);
	
   }
  else if(EEPROM.read(14)==0)
   {
    digitalWrite(R4,LOW);
	
   }

   
   
   
}
void save_data()
{
//____________________________________save eeprom___________________________________________
//saving data into the memory from the microprocessors pins.
   if(pieces[0]=="on" ||pieces[0]=="ON" )
   {
    EEPROM.write(11,1);
   }
  else if(pieces[0]=="off" ||pieces[0]=="OFF" )
   {
    EEPROM.write(11,0);
   }


   if(pieces[1]=="on" ||pieces[1]=="ON" )
   {
    EEPROM.write(12,1);
   }
  else if(pieces[1]=="off" ||pieces[1]=="OFF" )
   {
    EEPROM.write(12,0);
   }


   if(pieces[2]=="on" ||pieces[2]=="ON" )
   {
    EEPROM.write(13,1);
   }
  else if(pieces[2]=="off" ||pieces[2]=="OFF" )
   {
    EEPROM.write(13,0);
   }


   if(pieces[3]=="on" ||pieces[3]=="ON" )
   {
    EEPROM.write(14,1);
   }
  else if(pieces[3]=="off" ||pieces[3]=="OFF" )
   {
    EEPROM.write(14,0);
   }

   
}

void setup()
{
	
//To Enable the Onboard LED to act as a Status indicator light
pinMode(led,OUTPUT);
pinMode(R1,OUTPUT);
pinMode(R2,OUTPUT);
pinMode(R3,OUTPUT);
pinMode(R4,OUTPUT);
//Set the speed for communicating with the ESP8266 module
Serial.begin(115200);// signaling rate,board rate. To match the speed between wifi module and microprocessor.
//Send a series of flashes to show us we are in the bootup phase.
blinkcode ();
Serial.println("AT"); // to gain attention of the wifi module.
    // used initially to setup the setup the wifi module. Commented Now for presentation.
//Reset the ESP8266
//Serial.println("AT+RST");
//Wait for the WiFi module to bootup
//delay(800);
//Switch the chip to "client" mode
//Serial.println("AT+CWMODE=1");
//Connect to our Wireless network NOTE: change SSID to your wireless
//network and PASSWORD to the key used to join the network.
//Serial.println("AT+CWJAP=\"SSID\",\"PASSWORD\"");
//Once again blink some lights because it is fun!
blinkcode ();
  
}

void loop()
{
  //____________________________________________________________________________________
  digitalWrite(led,LOW);
  Serial.println("AT");
  delay(300); // to wait for the wifi module to respond.
//  Serial.println("AT+RST");
//  delay(4000);
//  Serial.println("AT");
//  delay(1000);
  Serial.println("AT");
  delay(500);
  Serial.println("AT+CIPMUX=1");//to select the channel of the wifi module.
  delay(1000);
  
//Open a connection to the web server
String cmd = "AT+CIPSTART=0,\"TCP\",\""; //to define the location of the website so that it can communicate with it.
cmd += DST_IP;
cmd += "\",80\r\n";
Serial.println(cmd);

//wait a little while for 'Linked'
delay(2000);//to wait for the website to respond.
Serial.println("AT+CIPSTART=?");
delay(800);



//This is our HTTP GET Request change to the page and server you want to load.
String cmd2  = "GET /s/message.html HTTP/1.0\r\n";//get method to fetch message.html page in s folder via http protocol.
       cmd2 += "Host: myiotprojects.com\r\n\r\n";
String url =" GET / HTTP/1.0Host: myprojectshub.comConnection: keep-aliveAccept: */*\r\n";
//The ESP8266 needs to know the size of the GET request
Serial.print("AT+CIPSEND=0,");
Serial.println((cmd2.length()));
//Serial.println("99");

//Look for the > prompt from the esp8266
if(Serial.find(">"))
{
//Send our http GET request
Serial.println(cmd2);
}
else
{
//Something didn't work...
Serial.println("AT+CIPCLOSE");
}

//Check the returned header & web page. Looking for a keyword. I used IOT

if (Serial.find("IOT"))
{
//IF we use an LED. 
//light to show the server is ONLINE
  // lcd.setCursor(0, 0);
  //  lcd.print("done it        ");
     digitalWrite(led,HIGH);//we indicate that we found IoT in the page.
      if(Serial.available() > 0)//To capture the data fetched from the webpage.
      {
//         lcd.clear(); // C LEARC DISPLAY IN CASE OF MESSAGE DISPLAY
         str = Serial.readStringUntil('*');

         
         for(i=0;i<str.length();i++)
         { 
           if (str.substring(i, i+1) == ",")
           {
              pieces[counter] = str.substring(lastIndex, i);
              lastIndex = i + 1;
              counter++;
           }
         }

         if (i == str.length() - 1) 
         {          
            pieces[counter] = str.substring(lastIndex, i);
         }



      //   for(i=0;i<4;i++)
       //  {   
  //          lcd.setCursor(0, i);      
  //          lcd.print(pieces[i]);           
       //  }

          counter = 0;
          lastIndex = 0;
                
        // lcd.setCursor(0, 0);
        // lcd.print(str);
      }
        
  
     
}
else
{
//If the string was not found then we can assume the server is offline therefore
//we should turn of the light.
  fastblinkcode ();
  digitalWrite(led,LOW);
 
 
}
save_data();
relay_action();
//Wait a second for everything to settle.
delay(5000);
delay(5000);
//closed our connection.
Serial.println("AT+CIPCLOSE");

//Introduce a delay timer before we finish and go back to the begining.
delay (500);
//delay (5000);
}
//**********************************************************************************
void blinkcode ()
{
//Simply loop a few times flashing the status light (this is used during boot up)
int i;
for (i = 1; i <= 10; i++){
delay (100);
    // digital write is used to write digitally to a pin.
digitalWrite(led,HIGH);
delay (100);
digitalWrite(led,LOW);
}
}

void fastblinkcode ()
{
//Simply loop a few times flashing the status light (this is used during boot up)
int i;
for (i = 1; i <= 10; i++){
delay (50);
digitalWrite(led,HIGH);
delay (150);
digitalWrite(led,LOW);
}
}





