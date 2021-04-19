#include <Adafruit_Arcada.h>
#include <CircularBuffer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_APDS9960.h>
#include <Adafruit_BMP280.h>
#include <PDM.h>

Adafruit_Arcada arcada;
Adafruit_LSM6DS33 lsm6ds33;
Adafruit_LIS3MDL lis3mdl;
Adafruit_SHT31 sht30;
Adafruit_APDS9960 apds9960;
Adafruit_BMP280 bmp280;
//extern PDMClass PDM;
#define WHITE_LED 43

// Color definitions
#define BACKGROUND_COLOR __builtin_bswap16(ARCADA_BLACK)
#define BORDER_COLOR __builtin_bswap16(ARCADA_BLUE)
#define PLOT_COLOR_1 __builtin_bswap16(ARCADA_PINK)
#define PLOT_COLOR_2 __builtin_bswap16(ARCADA_GREENYELLOW)
#define PLOT_COLOR_3 __builtin_bswap16(ARCADA_CYAN)
#define TITLE_COLOR __builtin_bswap16(ARCADA_WHITE)
#define TICKTEXT_COLOR __builtin_bswap16(ARCADA_WHITE)
#define TICKLINE_COLOR __builtin_bswap16(ARCADA_DARKGREY)

// Buffers surrounding the plot area
#define PLOT_TOPBUFFER 20
#define PLOT_LEFTBUFFER 40
#define PLOT_BOTTOMBUFFER 20
#define PLOT_W (ARCADA_TFT_WIDTH - PLOT_LEFTBUFFER)
#define PLOT_H (ARCADA_TFT_HEIGHT - PLOT_BOTTOMBUFFER - PLOT_TOPBUFFER)

// millisecond delay between samples
#define DELAY_PER_SAMPLE 50
void plotBuffer(GFXcanvas16 *_canvas, const char *title, 
                CircularBuffer<float, PLOT_W> &buffer1, 
                CircularBuffer<float, PLOT_W> &buffer2, 
                CircularBuffer<float, PLOT_W> &buffer3);

// Buffer for our plot data
CircularBuffer<float, PLOT_W> data_buffer;
CircularBuffer<float, PLOT_W> data_buffer2;
CircularBuffer<float, PLOT_W> data_buffer3;

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
//  PDM.onReceive(onPDMdata);
//  if (!PDM.begin(1, 16000)) {
//    Serial.println("**Failed to start PDM!");
//  }

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
