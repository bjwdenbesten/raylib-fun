#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define BALL_RADIUS 20

int main() {
  InitWindow(1200, 700, "halfpong");
  SetTargetFPS(60);
  SetRandomSeed(1902830138);

  int PADDLE_SPEED = 8;
  int BALL_SPEED = 4;
  int score = 0;
  bool increased = false;
  double timer = 0;

  bool gameStart = false;

  Rectangle paddle = {600, 500, 200, 40};

  // idk if this is the best way to do this but it works
  Rectangle left = {0, 0, 1, 700};
  Rectangle right = {1200, 0, 1, 700};
  Rectangle bottom = {0, 700, 1200, 1};
  Rectangle top = {0, 0, 1200, 1};

  int ballX = 600;
  int ballY = 200;

  int dirX = -1;
  int dirY = -1;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameStart) {
      const char *end_screen = "Press SPACE to Begin";
      int width = MeasureText(end_screen, 40);

      int centerTitleX = (1200 / 2) - (width / 2);
      DrawText(end_screen, centerTitleX, 300, 40, BLACK);

      if (score > 0) {
        const char *scoretext = TextFormat("Your Score Was: %d", score);
        int score_width = MeasureText(scoretext, 20);
        int centerScoreX = (1200 / 2) - (score_width / 2);
        DrawText(scoretext, centerScoreX, 350, 20, BLACK);
      }

      if (IsKeyPressed(KEY_SPACE)) {
        gameStart = true;
        score = 0;
        int randDir1 = GetRandomValue(0, 1);
        int randDir2 = GetRandomValue(0, 1);
        dirX = (randDir1 ? (1) : (-1));
        dirY = (randDir2 ? (1) : (-1));

        int randPosX = GetRandomValue(100, 1100);
        int randPosY = GetRandomValue(100, 300);
        ballX = randPosX;
        ballY = randPosY;
      }
      EndDrawing();
      continue;
    }

    DrawRectangleGradientEx(paddle, RED, BLUE, GREEN, YELLOW);
    DrawCircleGradient(ballX, ballY, BALL_RADIUS, BLUE, GREEN);

    DrawText(TextFormat("Score: %d", score), 20, 20, 20, BLACK);
    DrawText(TextFormat("Ball Speed: %d", BALL_SPEED), 20, 40, 20, BLACK);

    if (CheckCollisionCircleRec((Vector2){ballX, ballY}, BALL_RADIUS, bottom)) {
      PADDLE_SPEED = 8;
      BALL_SPEED = 4;
      timer = 0;
      gameStart = false;

      ballX = 600;
      ballY = 200;

      EndDrawing();
      continue;
    }

    // paddle movement checks
    if (!CheckCollisionRecs(paddle, right) && IsKeyDown(KEY_RIGHT)) {
      paddle.x += PADDLE_SPEED;
    }
    if (!CheckCollisionRecs(paddle, left) && IsKeyDown(KEY_LEFT)) {
      paddle.x -= PADDLE_SPEED;
    }

    // ball movement checks
    if (CheckCollisionCircleRec((Vector2){ballX, ballY}, BALL_RADIUS, top)) {
      dirY *= -1;
    }
    if (CheckCollisionCircleRec((Vector2){ballX, ballY}, BALL_RADIUS, right)) {
      dirX *= -1;
    }
    if (CheckCollisionCircleRec((Vector2){ballX, ballY}, BALL_RADIUS, left)) {
      dirX *= -1;
    }

    // if it hits the paddle
    if (CheckCollisionCircleLine(
            (Vector2){ballX, ballY}, BALL_RADIUS, (Vector2){paddle.x, paddle.y},
            (Vector2){paddle.x + paddle.width, paddle.y})) {
      dirY *= -1;
    }

    ballX += dirX * BALL_SPEED;
    ballY += dirY * BALL_SPEED;

    // speed increases every 5 seconds (this is definitely bad way to do it)
    if (((int)GetTime()) % 5 == 0) {
      if (!increased) {
        BALL_SPEED++;
        PADDLE_SPEED++;
        increased = true;
      }
    } else {
      increased = false;
    }

    timer += GetFrameTime();
    if (timer >= 1) {
      timer -= 1;
      score++;
    }

    EndDrawing();
  }

  return 0;
}
