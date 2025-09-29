#include <Wire.h>                                 
#include <LiquidCrystal_I2C.h>                    
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const char msg1[] = "POLREVO X FAETRO";   // 16 chars
const char msg2[] = "TRANSPORT ROBOT";    // 15 chars

unsigned long lastToggle = 0;
const unsigned long toggleInterval = 1000UL; // waktu berganti (ms) — ubah jika mau lebih cepat/lambat
bool showFirst = true;
bool headerInit = false;

void lcdSetup(){
  lcd.init();                                   
  lcd.backlight();      
  lcd.clear();         
}

// Cetak string di baris atas, pastikan membersihkan sisa karakter (tepat 16 kolom)
void printUpperFixed(const char *s) {
  lcd.setCursor(0, 0);
  lcd.print(s);
  lcd.setCursor(0, 1);
  lcd.print("PLAN NO: ");
  lcd.print(planNum);
  lcd.setCursor(0, 1);
  lcd.print("PLAN NO: ");
  lcd.print(planNum);
  size_t len = strlen(s);
  // bersihkan sisa kolom kalau pesan lebih pendek dari 16
  for (size_t i = len; i < 16; ++i) lcd.print(' ');
}

// Fungsi non-blocking untuk flip-flop header — panggil di loop()
void displayLCD() {
  unsigned long now = millis();

  // initial print sekali saja
  if (!headerInit) {
    printUpperFixed(msg1);
    lastToggle = now;
    showFirst = true;
    headerInit = true;
    return;
  }

  if (now - lastToggle >= toggleInterval) {
    lastToggle = now;
    showFirst = !showFirst;
    if (showFirst) printUpperFixed(msg1);
    else printUpperFixed(msg2);
  }
}
