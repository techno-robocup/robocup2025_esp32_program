#include <Arduino.h>
class Mymutex {
  public:
  Mymutex(portMUX_TYPE* mux);
  ~Mymutex();
  private:
  portMUX_TYPE *_mutex;
};