#include <my_mutex.hpp>
Mymutex::Mymutex(portMUX_TYPE *mux) {
  _mutex = mux;
  portENTER_CRITICAL(_mutex);
}
Mymutex::~Mymutex() { portEXIT_CRITICAL(_mutex); }