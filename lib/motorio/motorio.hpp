#ifndef __ROBOT__MOTORIO__HPP
#define __ROBOT__MOTORIO__HPP 1
#include <cstdlib>
#include <Arduino.h>
namespace robot {
  class motorio {
    public:
    motorio(const std::int8_t&,const std::int8_t&);
    void set_speed(int);
    void run_speed();
    private:
    std::int8_t IN1_PIN, IN2_PIN;
    int speed;
  };
}
#endif