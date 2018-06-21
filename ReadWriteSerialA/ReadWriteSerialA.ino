void setup(){
  
 Serial.begin(9600); 
}

void loop(){
 if(Serial.available()>0){
   char DatoSerial = Serial.read();
   if(DatoSerial == 'a'){
      int LecturaADC_A = analogRead(A0);
      Serial.write(LecturaADC_A>>8); 
      Serial.write(LecturaADC_A);
      int LecturaADC_B = analogRead(A1);
      Serial.write(LecturaADC_B>>8); 
      Serial.write(LecturaADC_B);
   }
 } 
  
}
