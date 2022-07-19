 float ph(){
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A4);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 3.3 / 1024 / 6;
  float ph_act = -5.70 * volt + calibration_value;
  Serial.print("pH Val:");

  Serial.println(ph_act);
  delay(1000);
  return ph_act;
 }
