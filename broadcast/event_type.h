#pragma once

#include <types.h>

typedef enum broadcast_event_type_t {
  EventTypeNone,
  EventTypeAll,
  EventTypeGoingToSleep,
  EventTypeWakeUp,

  // power
  ChargingOn,
  ChargingOff,

  // screen
  EventTypeChangeScreenBrightness,

  // storage
  EventMount,
  EventUnmount,

} broadcast_event_type_t;