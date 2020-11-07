

//*******************Check controller Connection********************************
 
void onConnection() {
   
  if (Ps3.isConnected()) {
    Serial.println("*******PS3 Controller connected*******");
  }
}

//*******************END Check controller Connection********************************
