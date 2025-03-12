#ifndef __ROBOT__MOTORIO__HPP
#define __ROBOT__MOTORIO__HPP 1
#include <cstdlib>
#include <Arduino.h>
namespace robot {
  class motorio {
    public:
    motorio(const std::int8_t&,const std::int8_t&,int*);
    void motor_brake();
    void set_speed(int);
    void run_speed();
    private:
    std::int8_t IN1_PIN, IN2_PIN;
    int speed;
    int *speed_array;
    std::uint8_t speed_variations = 20;
  };
}
#endif