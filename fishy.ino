#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define COLS 21
#define ROWS 8

struct Fish {
  float x;
  int   y;
  float speed;
  int   dir;    // 1=right, -1=left
};

struct Bubble {
  int x, y;
  bool active;
};

Fish fish[3];
Bubble bubbles[5];
int frame = 0;

// Fish sprites: right-facing and left-facing
const char* fishR = "><>";
const char* fishL = "<><";

void drawTank() {
  // Top border
  display.setCursor(0, 0);
  display.print(F("+-------------------+"));

  // Bottom border + sand
  display.setCursor(0, 56);
  display.print(F("+~^~^~^~^~^~^~^~^~^+"));
}

void drawSeaweed(int col, int row) {
  // Alternates lean each frame
  char weed = (frame % 4 < 2) ? '(' : ')';
  display.setCursor(col * 6, row * 8);
  display.print(weed);
}

void drawFish(Fish &f) {
  int px = (int)f.x * 6;
  int py = f.y * 8;
  display.setCursor(px, py);
  display.print(f.dir == 1 ? fishR : fishL);
}

void drawBubble(Bubble &b) {
  if (!b.active) return;
  display.setCursor(b.x * 6, b.y * 8);
  display.print('o');
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);

  // Init fish
  fish[0] = {1.0f, 2, 0.15f,  1};
  fish[1] = {15.0f, 4, 0.22f, -1};
  fish[2] = {8.0f, 6, 0.10f,  1};

  // Init bubbles
  for (int i = 0; i < 5; i++) {
    bubbles[i] = {random(1, 19), random(1, 6), false};
  }
}

void loop() {
  display.clearDisplay();

  drawTank();

  // Seaweed columns
  for (int row = 5; row >= 3; row--) drawSeaweed(3, row);
  for (int row = 5; row >= 4; row--) drawSeaweed(11, row);
  for (int row = 5; row >= 2; row--) drawSeaweed(17, row);

  // Update & draw fish
  for (int i = 0; i < 3; i++) {
    fish[i].x += fish[i].speed * fish[i].dir;
    if (fish[i].x > 17) fish[i].dir = -1;
    if (fish[i].x < 1)  fish[i].dir =  1;
    drawFish(fish[i]);
  }

  // Bubbles rise
  for (int i = 0; i < 5; i++) {
    if (!bubbles[i].active && random(40) == 0) {
      bubbles[i].x = random(1, 19);
      bubbles[i].y = 5;
      bubbles[i].active = true;
    }
    if (bubbles[i].active) {
      drawBubble(bubbles[i]);
      if (frame % 3 == 0) bubbles[i].y--;
      if (bubbles[i].y < 1) bubbles[i].active = false;
    }
  }

  display.display();
  frame++;
  delay(80);
}






