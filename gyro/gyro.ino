void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Hello! Arcada CLUE sensor plotter");
  //while (!Serial) yield();

  // Start TFT and fill black
  if (!arcada.arcadaBegin()) {
    Serial.print("Failed to begin");
    while (1) delay(10);
  }
  arcada.displayBegin();

  // Turn on backlight
  arcada.setBacklight(255);

  if (! arcada.createFrameBuffer(ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT)) {
    Serial.print("Failed to allocate framebuffer");
    while (1);
  }
  if (!apds9960.begin() || !lsm6ds33.begin_I2C() || !lis3mdl.begin_I2C() || 
      !sht30.begin(0x44) || !bmp280.begin()) {
      Serial.println("Failed to find CLUE sensors!");
      arcada.haltBox("Failed to init CLUE sensors");
  }
  /********** Check MIC */
  PDM.onReceive(onPDMdata);
  if (!PDM.begin(1, 16000)) {
    Serial.println("**Failed to start PDM!");
  }

  data_buffer.clear();
  data_buffer2.clear();
  data_buffer3.clear();
  pinMode(WHITE_LED, OUTPUT);
  digitalWrite(WHITE_LED, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
    sensors_event_t gyro;
    lsm6ds33.getEvent(NULL, &gyro, NULL);
    float x = gyro.gyro.x * SENSORS_RADS_TO_DPS;
    float y = gyro.gyro.y * SENSORS_RADS_TO_DPS;
    float z = gyro.gyro.z * SENSORS_RADS_TO_DPS;
    data_buffer.push(x);
    data_buffer2.push(y);
    data_buffer3.push(z);
    Serial.printf("Gyro: %f %f %f\n", x, y, z);
    plotBuffer(arcada.getCanvas(), "Gyro (dps)",
               data_buffer, data_buffer2, data_buffer3);

}
