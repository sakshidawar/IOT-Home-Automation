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





