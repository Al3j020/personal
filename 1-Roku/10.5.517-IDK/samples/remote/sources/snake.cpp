// Copyright 2021 Roku, Inc.  All rights reserved.
#include "snake.h"
#include "remote.h"
#include "screen.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
using namespace std::chrono_literals;
namespace {
RemoteControlRef REMOTE;

struct Game : std::vector<bool>
{
  using Ref = std::shared_ptr<Game>;
  static Ref Create();

  ScreenRef GL;
  const int limit_x;
  const int limit_y;
  const float cw;
  const float ch;
  int snake_x;       // (x,y) coordinate of
  int snake_y;       // head of our snake.
  int snake_dx;      // Direction where the
  int snake_dy;      // snake is going next.
  float fStuff;      // Change the red color
  float fStuffDelta; // to make output interesting.
  std::atomic<bool> bRunning;
  std::atomic<bool> bGameOver;
  std::thread threadGame;
  std::chrono::steady_clock::time_point timeLastMoved;
  std::chrono::steady_clock::duration delayNextMove;
  GLuint vbo;
  GLuint ibo;
  GLuint uv;

  void draw();
  void updateGameState();
  void over();
  void restart();
  bool setOccupied(int x, int y);
  bool IsOccupied(int x, int y);

  Game(int, int);
  ~Game();

private:
};

Game::Ref
Game::Create()
{
  return std::make_shared<Game>(128, 72);
}

Game::Game(int w, int h)
  : std::vector<bool>((w) * (h))
  , GL(Screen::Create())
  , limit_x(128)
  , limit_y(72)
  , cw(0.0156225f)
  , ch(0.0277777f)
  , fStuff(1.0f)
  , fStuffDelta(0.01f)
  , bGameOver{ false }
{
  auto setup = GL->push([this](int) {
    glm::vec3 vertices[4] = { { -1.0f, -1.0f, 0.0f },
                              { -1.0f + cw, -1.0f, 0.0f },
                              { -1.0f + cw, -1.0f + ch, 0.0f },
                              { -1.0f, -1.0f + ch, 0.0f } };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(LOCATION_VPOS);
    glVertexAttribPointer(LOCATION_VPOS, 3, GL_FLOAT, GL_FALSE, 0, 0);

    uint16_t indices[6] = { 0, 1, 2, 2, 3, 0 };
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  });
  setup.wait();

  threadGame = std::thread([this] {
    bRunning = true;
    while (bRunning) {
      std::this_thread::sleep_for(16ms);
      updateGameState();
      if (bGameOver) {
        if ((std::chrono::steady_clock::now() - timeLastMoved) > 5s) {
          bGameOver = false;
          restart();
        }
      }
      draw();
    }
  });

  restart();
}

Game::~Game()
{
  bRunning = false;
  auto cleanup = GL->push([this](int) {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &uv);
  });
  cleanup.wait();
  GL.reset();

  if (threadGame.joinable()) {
    threadGame.join();
  }
}

void
Game::draw()
{
  GL->push([this](int) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform1f(GL->shader.uStuff, fStuff);
    fStuff += fStuffDelta;
    if (fStuff > 1.0f) {
      fStuff = 1.0f;
      fStuffDelta = -fStuffDelta;
    } else if (fStuff < 0.0f) {
      fStuff = 0.0f;
      fStuffDelta = -fStuffDelta;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    for (int i = 0; i < limit_x; ++i) {
      for (int j = 0; j < limit_y; ++j) {
        if (IsOccupied(i, j)) {
          auto t =
            glm::translate(glm::mat4(1.0f), glm::vec3(i * cw, j * ch, 0.0f));
          glUniformMatrix4fv(GL->shader.uMVP, 1, GL_TRUE, glm::value_ptr(t));
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
        }
      }
    }
  });
  auto vsync = GL->swap();
  vsync.wait();
}

void
Game::over()
{
  for (auto&& cell : *this) {
    cell = true;
  }
}

void
Game::restart()
{
  for (auto&& cell : *this) {
    cell = false;
  }

  for (int i = 0; i < limit_x; ++i) {
    setOccupied(i, 0);
    setOccupied(i, (limit_x * (limit_y - 1)));
  }
  for (int i = 0; i < limit_y; ++i) {
    setOccupied(0, i);
    setOccupied((limit_x - 1), i);
  }
  snake_x = 64;
  snake_y = 36;
  snake_dx = 1;
  snake_dy = 0;
  timeLastMoved = std::chrono::steady_clock::now();
  delayNextMove = 168ms;
  fprintf(stderr, "GAME RESTARTED\n");
}

bool
Game::setOccupied(int x, int y)
{
  auto old = IsOccupied(x, y);
  x = std::min<int>(x, limit_x - 1);
  x = std::max<int>(x, 0);
  y = std::min<int>(y, limit_y - 1);
  y = std::max<int>(y, 0);
  (*this)[y * limit_x + x] = true;
  return old;
}

bool
Game::IsOccupied(int x, int y)
{
  x = std::min<int>(x, limit_x - 1);
  x = std::max<int>(x, 0);
  y = std::min<int>(y, limit_y - 1);
  y = std::max<int>(y, 0);
  auto b = (*this)[y * limit_x + x];
  return b;
}

void
Game::updateGameState()
{
  if (bGameOver) {
    return; // No game state to update, waiting for restart.
  }

  const auto now = std::chrono::steady_clock::now();
  if (now > (timeLastMoved + delayNextMove)) {
    delayNextMove = delayNextMove - 500ns; // Make snake go a little faster.
    timeLastMoved = now;
    snake_x += snake_dx;
    snake_y += snake_dy;
    bGameOver = setOccupied(snake_x, snake_y);
    if (bGameOver) {
      over();
    }
  }
}
} // anonymous namespace

void
RunSnakeDemo()
{
  auto REMOTE = RemoteControl::Create();
  if (REMOTE) {
    auto game = Game::Create();

    // Set this callback to get remote input events.
    REMOTE->setButtonCallback([game](int32_t keycode, bool released) {
      fprintf(stderr, "0x%08x %s\n", keycode, released ? "up" : "down");
      fflush(stderr);
      if (!released) {
        return; // Our snake only changes direction on key up.
      }

      switch (keycode) {
        case 0x0000001: // UP
          game->snake_dx = 0;
          game->snake_dy = 1;
          game->updateGameState();
          break;
        case 0x0000002: // DOWN
          game->snake_dx = 0;
          game->snake_dy = -1;
          game->updateGameState();
          break;
        case 0x0000004: // LEFT
          game->snake_dx = -1;
          game->snake_dy = 0;
          game->updateGameState();
          break;
        case 0x0000008: // RIGHT
          game->snake_dx = 1;
          game->snake_dy = 0;
          game->updateGameState();
          break;
        case 0x0000010:
          break; // OK
        case 0x0000020:
          break; // PLAY/PAUSE
        case 0x0000040:
          break; // REWIND
        case 0x0000080:
          break; // FAST FORWARD
        case 0x0000100:
          break; // BACK
        case 0x0000200:
          break; // STAR
        case 0x0000400:
          break; // INSTANT REPLAY
      }
    });

    REMOTE->runEventLoop();
  }
}

void
ExitSnakeDemo()
{
  if (REMOTE) {
    REMOTE->exitEventLoop();
  }
}
