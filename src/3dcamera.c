#include <math.h>
#include <raylib.h>
#include <stdio.h>

#define HEIGHT 700
#define WIDTH 1200
#define SENSITIVITY 0.003
#define CAMERA_SPEED 0.30
#define MOVEMENT_SPEED 0.5

// FPS LIKE MOVEMENT FROM SCRATCH

Vector3 normalizeVector(Vector3 v3) {
  float magnitude = sqrtf(v3.x * v3.x + v3.y * v3.y + v3.z * v3.z);
  if (magnitude == 0)
    return (Vector3){0.0f, 0.0f, 0.0f};
  return (Vector3){v3.x / magnitude, v3.y / magnitude, v3.z / magnitude};
}

Vector3 sumVector3(Vector3 v1, Vector3 v2) {
  return (Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 multiplication(Vector3 v1, float constant) {
  return (Vector3){v1.x * constant, v1.y * constant, v1.z * constant};
}

Vector3 crossProduct(Vector3 v1, Vector3 v2) {
  return (Vector3){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x};
}

int main() {
  InitWindow(WIDTH, HEIGHT, "3D Camera");
  SetTargetFPS(60);

  float cameraX = 0.0f;
  float cameraY = 10.0f;
  float cameraZ = 10.0f;

  float cameraXTarget = 0.0f;
  float cameraYTarget = 0.0f;
  float cameraZTarget = 0.0f;

  float yaw = 0.0f;   // left right movement
  float pitch = 0.0f; // up down movement

  Vector3 shrek = {0.0f, 0.0f, 0.0f};
  Vector3 tripleT = {-15.0f, 0.0f, 0.0f};

  Model shrek_model = LoadModel("../models/shrek.glb");
  Model tripleT_model = LoadModel("../models/tripleT.glb");

  Vector3 Initial = normalizeVector(
      (Vector3){shrek.x - cameraX, shrek.y - cameraY, shrek.z - cameraZ});

  yaw = atan2f(Initial.x, Initial.z);
  pitch = asinf(Initial.y);

  SetMousePosition(WIDTH / 2, HEIGHT / 2);
  DisableCursor();

  while (!WindowShouldClose()) {
    // we define the camera in here to move it
    Camera3D camera = {0};
    camera.position =
        (Vector3){cameraX, cameraY, cameraZ}; // where the camera is positioned
    camera.target = (Vector3){cameraXTarget, cameraYTarget,
                              cameraZTarget}; // where we're looking
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};  // this defines that +Y is up
    camera.fovy = 45.0f;

    // get the mouse changes between frames.
    Vector2 mouseChange = GetMouseDelta();

    yaw -= mouseChange.x * SENSITIVITY;
    pitch -= mouseChange.y * SENSITIVITY;

    float limit = 89.0f * DEG2RAD;

    // stop wonky camera positions
    if (pitch < -limit) {
      pitch = -limit;
    }

    if (pitch > limit) {
      pitch = limit;
    }

    // check out
    // https://math.stackexchange.com/questions/2618527/converting-from-yaw-pitch-roll-to-vector
    // for the equation
    Vector3 direction = {cosf(pitch) * sinf(yaw), sinf(pitch),
                         cosf(pitch) * cosf(yaw)};

    // we normalize the vector to ensure its not too big
    direction = normalizeVector(direction);

    // ensure we are moving the camera target relative to the camera position,
    // not just randomly
    camera.target = sumVector3(camera.position, direction);

    Vector3 right_direction =
        normalizeVector(crossProduct(direction, camera.up));

    // input stuff
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
      cameraY -= CAMERA_SPEED;
    }

    if (IsKeyDown(KEY_SPACE)) {
      cameraY += CAMERA_SPEED;
    }

    if (IsKeyDown(KEY_W)) {
      cameraX += direction.x * MOVEMENT_SPEED;
      cameraZ += direction.z * MOVEMENT_SPEED;
    }

    if (IsKeyDown(KEY_S)) {
      cameraX -= direction.x * MOVEMENT_SPEED;
      cameraZ -= direction.z * MOVEMENT_SPEED;
    }

    if (IsKeyDown(KEY_D)) {
      cameraX += right_direction.x * MOVEMENT_SPEED;
      cameraY += right_direction.y * MOVEMENT_SPEED;
      cameraZ += right_direction.z * MOVEMENT_SPEED;
    }

    if (IsKeyDown(KEY_A)) {
      cameraX -= right_direction.x * MOVEMENT_SPEED;
      cameraY -= right_direction.y * MOVEMENT_SPEED;
      cameraZ -= right_direction.z * MOVEMENT_SPEED;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // Draw any 3D thing you want here ...

    DrawGrid(100, 1.0f);
    DrawModel(shrek_model, shrek, 0.05, WHITE);
    DrawModel(tripleT_model, tripleT, 1, WHITE);

    EndMode3D();

    DrawText("To Move: WASD", 30, 30, 20, BLACK);
    DrawText("DOWN: Left Shift | UP: Space", 30, 60, 20, BLACK);
    DrawText("EXIT: Esc", 30, 90, 20, BLACK);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
