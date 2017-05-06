#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
const uint16_t COLORS[7] = {
  BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW,WHITE
};

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

const int SY = 32;
const int SX = 24;
const int SR = 10;
bool cs[SY][SX];
bool prev[SY][SX]; 
int gen = 1;

void setup() {
  tft.reset();
  tft.begin();
  tft.fillScreen(BLACK);
  lifegame_init();
  lifegame_example();
}

void loop() {
  lifegame_next();
  //delay(100);
}

void lifegame_example() {
  int my = SY / 2;
  int mx = SX / 2;
  cs[my+-1][mx+-1] = 1;
  cs[my+1][mx+-1] = 1;
  cs[my+-2][mx+0] = 1;
  cs[my+-1][mx+1] = 1;
  cs[my+2][mx+1] = 1;
  cs[my+1][mx+2] = 1;
  cs[my+2][mx+2] = 1;
  cs[my+3][mx+2] = 1;
}

void lifegame_init() {
  for (int y = 0; y < SY; y++) {
    for (int x = 0; x < SX; x++) {
      cs[y][x] = false;
      prev[y][x] = false;
    }
  }
}

void lifegame_copy() {
  for (int y = 0; y < SY; y++) {
    for (int x = 0; x < SX; x++) {
      prev[y][x] = cs[y][x];
    }
  }
}

void lifegame_next() {
  lifegame_copy();

  for (int y = 0; y < SY; y++) {
    for (int x = 0; x < SX; x++) {
      int cn = 0;
      for (int yy = y-1; yy <= y+1; yy++) {
        for (int xx = x-1; xx <= x+1; xx++) {
          if ((xx != x || yy != y) && xx >= 0 && yy >= 0 && xx < SX && yy < SX) {
            if (prev[yy][xx]) {
              cn += 1;
            }
          }
        }
      }

      bool c = prev[y][x];

      if (c == true && cn == 2) {
        cs[y][x] = true;
      }
      else if (c == true && cn == 3) {
        cs[y][x] = true;
      }
      else if (c == false && cn == 3) {
        cs[y][x] = true;
      }
      else {
        cs[y][x] = false;
      }

      if (cs[y][x] && !c) {
        tft.fillRect(x * SR, y * SR, SR - 1, SR - 1, GREEN);
      }
      else if (!cs[y][x] && c) {
        tft.fillRect(x * SR, y * SR, SR - 1, SR - 1, BLACK);
      }
    }
  }
}
