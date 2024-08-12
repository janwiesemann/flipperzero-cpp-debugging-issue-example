#include <furi.h>
#include <string>
#include <vector>

class Platform {
public:
  Platform() { printf("Platform: create\n"); }
  virtual ~Platform() { printf("Platform: kill\n"); }
  virtual void Draw() = 0;
};

class NormalPlatform : public Platform {
public:
  NormalPlatform() : Platform() {}

  void Draw() override {
     printf("----\n"); // Breakpoint not hit
     __asm__("BKPT"); //if this is beeing hit, VS-Code halts at line 33
  }
};

class BrokenPlatform : public Platform {
  void Draw() override {
     printf("-\\ _/-\n"); // Breakpoint not hit
  }
};

class Game {
  std::vector<Platform *> platforms;

public:
  Game() {
    for (int i = 0; i < 10; i++) {
      Platform *p = nullptr;
      if (i % 2 == 0)
        p = new NormalPlatform();
      else
        p = new BrokenPlatform();

      platforms.push_back(p);
    }
  }

  ~Game() {
    for (std::vector<Platform *>::iterator i = platforms.begin();
         i != platforms.end(); i++) {
      Platform *p = *i;
      delete p;
    }
  }

  void Tick() {
    for (std::vector<Platform *>::iterator i = platforms.begin();
         i != platforms.end(); i++) {
      Platform *p = *i;
      p->Draw();
    }
    printf("\n\n-------------------------\n\n");
  }
};

extern "C" {
int32_t flipperzero_cpp_debugging_issue_example_app(void *p) {
  UNUSED(p);

  Game game;
  for (int i = 0; i < 5; i++) {
    game.Tick();
  }

  return SUCCESS;
}
};
