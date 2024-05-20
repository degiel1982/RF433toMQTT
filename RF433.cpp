#include "RF433.h"

void RF433::setup(int receiver_pin) {
  if (receiver_pin != 0) {
    rf433.enableReceive(receiver_pin);
  }
}

unsigned long RF433::received_code(unsigned long debounce_time) {
  if (rf433.available()) {
    unsigned long rfcode = rf433.getReceivedValue();
    if (((millis() - old_time) > debounce_time) || rfcode != old_code) {
      old_code = rfcode;
      old_time = millis();
      rf433.resetAvailable();
      return rfcode;
    }
    rf433.resetAvailable();
  }
  return 0;
}
