int tempa(){
  //Code for Temperature Sensor and FIrebase.
  sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
  int temp= sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" C");

  if(temp >= 27){
    digitalWrite(r3, LOW);
    Serial.println("Enfriando");
  }

  if(temp <= 25){
    digitalWrite(r3, HIGH);
    Serial.println("Calentado");
  }

  return temp;
}