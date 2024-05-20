#ifndef RF433_H
#define RF433_H

#include <RCSwitch.h>

class RF433 {
public:
  void setup(int receiver_pin = 0);
  unsigned long received_code(unsigned long debounce_time = 0);

private:
  unsigned long old_time = 0;
  unsigned long old_code = 0;
  RCSwitch rf433;
};

#endif // RF433_H
