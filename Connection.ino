

//*******************Check controller Connection********************************
 
void onConnection() {
   
  if (Ps3.isConnected()) 
  {
  Serial.println();
  Serial.println();
  Serial.println("********************************");
  Serial.println("*** PS3 Controller Connected ***");
  Serial.println("********************************");
  Serial.println();
  Serial.println();
  }
 }

//*******************END Check controller Connection********************************
