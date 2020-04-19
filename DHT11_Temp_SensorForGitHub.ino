// *****************************************
// James Allen April 2020
// DHT 11 Temperature Sensor Code for Display on 128*64 OLED Display
// *****************************************

#include <DHT.h>
#include "U8glib.h"

#define DHTPIN 2          
#define DHTTYPE DHT11     

DHT dht(DHTPIN, DHTTYPE);

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK); 

bool first = true;
float hum = 0.0;
float temp = 0.0;
float hIndex = 0.0;
bool dht_test(float* humPerc, float* tempF, float* heatIndex);

void setup(void)
{
  Serial.begin(9600);
  dht.begin();
  first = true;

  // assign default color value
  if (u8g.getMode() == U8G_MODE_R3G3B2)
  {
    u8g.setColorIndex(255);    
  }
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
  {
    u8g.setColorIndex(3);         
  }
  else if (u8g.getMode() == U8G_MODE_BW)
  {
    u8g.setColorIndex(1);         
  }
  else if (u8g.getMode() == U8G_MODE_HICOLOR)
  {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  dht_test(&hum, &temp, &hIndex);
}
void HumMeter(float* humPerc, float* temp, float* heatIndex)
{
  u8g.setFont(u8g_font_fub11);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  u8g.drawStr(4, 0, "Hum %");
  u8g.setPrintPos(80, 0);
  u8g.print(*humPerc);
#ifdef METRIC
  // if metric system, display Celsius
  u8g.drawStr(4, 20, "Temp C");
#else
  //display Farenheit
  u8g.drawStr(4, 20, "Temp C");
#endif

  u8g.setPrintPos(80, 20);
  u8g.print(*temp);
  u8g.drawStr(4, 40, "Heat Ind");
  u8g.setPrintPos(80, 40);
  u8g.print(*heatIndex);
}

void loop(void)
{
  bool result = dht_test(&hum, &temp, &hIndex);

  if (first)
  {
    // skip displaying readings first time, as its stale data.
    first = false;
  }
  else
  {
    if(result == false)
    {
      u8g.firstPage();
      do 
      {
        // sensor error
        u8g.setFont(u8g_font_fub11);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawStr(10, 30, "Sensor Error");
      }
      while (u8g.nextPage());
    }
    else
    {
      u8g.firstPage();
      do
      {
        HumMeter(&hum, &temp, &hIndex);
      }
      while (u8g.nextPage());
    }
  }
}
bool dht_test(float* humPerc, float* temp, float* heatIndex)
{
  // Wait a few seconds between measurements.
  delay(2000);
  *humPerc = 0;
  *temp = 0;
  *heatIndex = 0;
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(false);
)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  *humPerc = h;
#ifdef METRIC
  *temp = t;
  *heatIndex = hic;
#else
  *temp = f;
  *heatIndex = hif;
#endif
  return true;
}
