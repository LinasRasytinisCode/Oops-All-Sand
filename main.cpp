#include "raylib.h"
#include <cstdlib>
#include <omp.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS_TARGET = 120;

const int CANVAS_WIDTH = 320;
const int CANVAS_HEIGHT = 180;

enum Element { AIR = 0, SAND = 1, WATER = 2 };

void UpdateColor(int x, int y, Element pixelArray[CANVAS_HEIGHT][CANVAS_WIDTH],
                 Color *pixels) {
  int index = y * CANVAS_WIDTH + x;

  switch (pixelArray[y][x]) {
  case SAND:
    pixels[index] = Color{215, 232, 199, 255};
    break;
  case WATER:
    pixels[index] = BLUE;
    break;
  case AIR:
    pixels[index] = BLANK;
    break;
  }
}

void SwitchPlaces(int x, int y, int new_x, int new_y,
                  Element pixelArray[CANVAS_HEIGHT][CANVAS_WIDTH],
                  Color *pixels) {
  auto temp = pixelArray[y][x];
  pixelArray[y][x] = pixelArray[new_y][new_x];
  pixelArray[new_y][new_x] = temp;

  UpdateColor(x, y, pixelArray, pixels);
  UpdateColor(new_x, new_y, pixelArray, pixels);
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling sand");
  SetTargetFPS(FPS_TARGET);

  Element pixelArray[CANVAS_HEIGHT][CANVAS_WIDTH] = {AIR};

  Color *pixels = (Color *)malloc(CANVAS_WIDTH * CANVAS_HEIGHT * sizeof(Color));

  for (int y = 0; y < CANVAS_HEIGHT; y++) {
    for (int x = 0; x < CANVAS_WIDTH; x++) {
      int index = y * CANVAS_WIDTH + x;
      int randomNumber = rand() % 3;
      switch (randomNumber) {
      case 0:
        pixelArray[y][x] = AIR;
        // pixels[index] = BLANK;
        break;
      case 1:
        pixelArray[y][x] = SAND;
        // pixels[index] = Color{215, 232, 199, 255};
        break;
      case 2:
        pixelArray[y][x] = WATER;
        // pixels[index] = BLUE;
        break;
      }
    }
  }

  Image display = GenImageColor(CANVAS_WIDTH, CANVAS_HEIGHT, BLANK);

  Texture2D displayTexture = LoadTextureFromImage(display);

  while (!WindowShouldClose()) {

    for (int x = 0; x < CANVAS_WIDTH; x++) {
      for (int y = CANVAS_HEIGHT - 1; y >= 0; y--) {

        Element current = pixelArray[y][x];
        if (current == AIR)
          continue;

        if (y < CANVAS_HEIGHT - 1) {
          if (current == SAND) {
            if (pixelArray[y + 1][x] == AIR)
              SwitchPlaces(x, y, x, y + 1, pixelArray, pixels);
            else if (pixelArray[y + 1][x] == WATER)
              SwitchPlaces(x, y, x, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[y + 1][x - 1] == WATER)
              SwitchPlaces(x, y, x - 1, y + 1, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[y + 1][x + 1] == WATER)
              SwitchPlaces(x, y, x + 1, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[y + 1][x - 1] == AIR)
              SwitchPlaces(x, y, x - 1, y + 1, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[y + 1][x + 1] == AIR)
              SwitchPlaces(x, y, x + 1, y + 1, pixelArray, pixels);
          } else if (current == WATER) {
            if (pixelArray[y + 1][x] == AIR)
              SwitchPlaces(x, y, x, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[y + 1][x - 1] == AIR)
              SwitchPlaces(x, y, x - 1, y + 1, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[y + 1][x + 1] == AIR)
              SwitchPlaces(x, y, x + 1, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[y][x - 1] == AIR)
              SwitchPlaces(x, y, x - 1, y, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[y][x + 1] == AIR)
              SwitchPlaces(x, y, x + 1, y, pixelArray, pixels);
          }
        }
      }
    }
    //This is terrible.
    

    BeginDrawing();
    ClearBackground(BLACK);
    UpdateTexture(displayTexture, pixels);
    DrawTextureEx(displayTexture, Vector2{0, 0}, 0, 4.0f, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}