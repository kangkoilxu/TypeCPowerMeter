/*
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <INA226_asukiaaa.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/**
 * R7 SCL ,  
 * R8 SDA , 
 * 5, SCL ,
 * 4, SDA ,
 */
const uint16_t ina226calib = INA226_asukiaaa::calcCalibByResisterMilliOhm(10); // Max 5120 milli ohm
INA226_asukiaaa voltCurrMeter(INA226_ASUKIAAA_ADDR_A0_GND_A1_GND, ina226calib);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  

void setup(void) {
  
  Serial.begin(115200);
  
  u8g2.begin();

  if (voltCurrMeter.begin() != 0) {
    Serial.println("Failed to begin INA226");
  }
}

void loop(void) {
  
  int16_t ma, mv, mw;
  char buf[50];
  
  // This problem applies only to full buffer mode
  u8g2.sendF("c", 0x0a7);
  u8g2.clearBuffer();  
  u8g2.setFontMode(1);
  
  if (voltCurrMeter.readMV(&mv) == 0) {
    Serial.println(String(mv) + "mV");
    u8g2.setFont(u8g2_font_fub20_tf);
    u8g2.setCursor(20,20);
    sprintf(buf,"%d.%03dV",mv/1000,mv%1000);
    u8g2.print(buf);
  } else {
    Serial.println("Cannot read voltage.");
  }
  if (voltCurrMeter.readMA(&ma) == 0) {
    Serial.println(String(ma) + "mA");
    u8g2.setFont(u8g2_font_cu12_tr);  
    u8g2.setCursor(0,32);
    memset(buf,0,50);
    sprintf(buf,"%d.%03dA",ma/1000,ma%1000);
    u8g2.print(buf);  
  } else {
    Serial.println("Cannot read current.");
  }
  if (voltCurrMeter.readMW(&mw) == 0) {
    Serial.println(String(mw) + "mW");

    u8g2.setFont(u8g2_font_cu12_tr);  
    u8g2.setCursor(60,32);
    memset(buf,0,50);
    sprintf(buf,"%d.%03dW",mw/1000,mw%1000);
    u8g2.print(buf);  
  } else {
    Serial.println("Cannot read watt.");
  } 

  u8g2.sendBuffer();
  delay(500);  
}
