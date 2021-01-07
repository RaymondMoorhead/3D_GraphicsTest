#include "Graphics/Graphics.h"
#include <chrono>

#define TIME_NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

int main(int argc, char* argv[])
{
  float delta_time;
  long long cur_time = TIME_NOW, old_time_;

  GRAPHICS.Initialize();

  do
  {
    // calculate delta time
    old_time_ = cur_time;
    cur_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    delta_time = float(double(cur_time - old_time_) / 1000.0);
  } while (GRAPHICS.Update(delta_time));

  return 0;
}