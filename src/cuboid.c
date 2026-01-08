#include <math.h>
#include <raylib.h>
#include <stdio.h>

/* 3D floating cuboid */

int main() {
  int w_width = 1200;
  int w_height = 700;
  InitWindow(w_width, w_height, "2dCube");
  SetTargetFPS(60);

  Vector2 points[8] = {0};
  float zpositions[8] = {0};

  float radian = 0;
  int focal_length = 300;

  int centerX = 600;
  int centerY = 350;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("3D cuboid, totally", 50, 50, 20, GREEN);

    // create all the vertices of the cubes
    for (int i = 0; i < 8; i++) {
      float xpos = cos(radian + (PI / 2) * i);
      float zpos = 3 + sin(radian + (PI / 2) * i) + (i < 4 ? 0.0f : 0.2f);
      float ypos = (i < 4 ? 1 : -0.8);

      float ratio = (0.2 / zpos) * focal_length;

      xpos = (xpos / zpos) * focal_length + centerX;
      ypos = (ypos / zpos) * focal_length + centerY;

      Vector2 o_pts = {xpos, ypos};
      zpositions[i] = zpos;

      points[i] = o_pts;

      DrawCircle(xpos, ypos, ratio, GREEN);
    }

    // draw the lines connecting bottom, and top
    for (int i = 0; i < 8; i++) {
      int thickness = 10;
      if (i < 4) {
        float average = (zpositions[i] + zpositions[(i + 1) % 4]) / 2;
        float thickness = (0.2 / average) * focal_length;
        DrawLineEx(points[i], points[(i + 1) % 4], thickness, GREEN);
      } else {
        int other = i + 1;
        if (other == 8)
          other = 4;

        float average = (zpositions[i] + zpositions[other]) / 2;
        float thickness = (0.2 / average) * focal_length;

        DrawLineEx(points[i], points[other], thickness, GREEN);
      }
    }

    // draw connecting lines between top and bottom faces
    for (int i = 0; i < 4; i++) {
      int first = i;
      int second = first + 4;
      float thickness = 0.8 * (0.2 / zpositions[i]) * focal_length;
      DrawLineEx(points[first], points[second], thickness, GREEN);
    }

    // ensure that our radians wrap around
    radian += PI / 150;
    if (radian >= 2 * PI) {
      radian -= 2 * PI;
    }
    EndDrawing();
  }

  return 0;
}
