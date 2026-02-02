#include "raylib.h"
#include <cstdlib>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS_TARGET = 120;

const int CANVAS_WIDTH = 320;
const int CANVAS_HEIGHT = 180;

const int CANVAS_ARRAY_SIZE = CANVAS_HEIGHT*CANVAS_WIDTH;

enum Element { AIR = 0, SAND = 1, WATER = 2 };

inline int idx(int x, int y){
  return y * CANVAS_WIDTH + x;
}

void UpdateColor(int x, int y, Element pixelArray[CANVAS_HEIGHT*CANVAS_WIDTH],
                 Color *pixels) {
  int index = idx(x, y);

  switch (pixelArray[index]) {
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
                  Element pixelArray[CANVAS_ARRAY_SIZE],
                  Color *pixels) {
  auto temp = pixelArray[idx(x, y)];
  pixelArray[idx(x, y)] = pixelArray[idx(new_x, new_y)];
  pixelArray[idx(new_x, new_y)] = temp;

  UpdateColor(x, y, pixelArray, pixels);
  UpdateColor(new_x, new_y, pixelArray, pixels);
}

void InitializeCanvas(Element pixelArray[CANVAS_ARRAY_SIZE], Color *pixels){
  for(int i = 0; i < CANVAS_ARRAY_SIZE; i++){
    int randomNumber = rand() % 3;
    switch (randomNumber) {
      case 0:
        break;
      case 1:
        pixelArray[i] = SAND;
        break;
      case 2:
        pixelArray[i] = WATER;
        break;
    }
  }
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling sand");
  SetTargetFPS(FPS_TARGET);

  Element pixelArray[CANVAS_ARRAY_SIZE] = {AIR};

  Color *pixels = (Color *)malloc(CANVAS_ARRAY_SIZE * sizeof(Color));

  InitializeCanvas(pixelArray, pixels);

  Image display = GenImageColor(CANVAS_WIDTH, CANVAS_HEIGHT, BLANK);

  Texture2D displayTexture = LoadTextureFromImage(display);

  while (!WindowShouldClose()) {

    for (int x = 0; x < CANVAS_WIDTH; x++) {
      for (int y = CANVAS_HEIGHT - 1; y >= 0; y--) {

        Element current = pixelArray[idx(x, y)];
        if (current == AIR)
          continue;

        if (y < CANVAS_HEIGHT - 1) {
          if (current == SAND) {
            if (pixelArray[idx(x, y+1)] == AIR)
              SwitchPlaces(x, y, x, y + 1, pixelArray, pixels);
            else if (pixelArray[idx(x, y+1)] == WATER)
              SwitchPlaces(x, y, x, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[idx(x-1, y+1)] == WATER)
              SwitchPlaces(x, y, x - 1, y + 1, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[idx(x+1, y+1)] == WATER)
              SwitchPlaces(x, y, x + 1, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[idx(x-1, y+1)] == AIR)
              SwitchPlaces(x, y, x - 1, y + 1, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[idx(x+1, y+1)] == AIR)
              SwitchPlaces(x, y, x + 1, y + 1, pixelArray, pixels);
          } else if (current == WATER) {
            if (pixelArray[idx(x, y+1)] == AIR)
              SwitchPlaces(x, y, x, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[idx(x-1, y+1)] == AIR)
              SwitchPlaces(x, y, x - 1, y + 1, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[idx(x+1, y+1)] == AIR)
              SwitchPlaces(x, y, x + 1, y + 1, pixelArray, pixels);
            else if (x > 0 && pixelArray[idx(x-1, y)] == AIR)
              SwitchPlaces(x, y, x - 1, y, pixelArray, pixels);
            else if (x < CANVAS_WIDTH - 1 && pixelArray[idx(x+1, y)] == AIR)
              SwitchPlaces(x, y, x + 1, y, pixelArray, pixels);
          }
        }
      }
    }
    // This is terrible.

    BeginDrawing();
    ClearBackground(BLACK);
    UpdateTexture(displayTexture, pixels);
    DrawTextureEx(displayTexture, Vector2{0, 0}, 0, 4.0f, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}