int level(){
  //Code for Water Level with led RGB and Firebase.
  int agua = digitalRead(nivel);

  if(agua == HIGH){
    Serial.println("Vacio");
    digitalWrite(r1, LOW);  
    WiFiDrv::analogWrite(25, 0);
    WiFiDrv::analogWrite(26, 255);
    WiFiDrv::analogWrite(27, 0);
  }
  if(agua == LOW){
    Serial.println("Lleno");
    digitalWrite(r1, HIGH);
    WiFiDrv::analogWrite(25, 255); //GREEN
    WiFiDrv::analogWrite(26, 0);   //RED
    WiFiDrv::analogWrite(27, 0);
  }

  return agua;
}