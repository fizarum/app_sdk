#ifndef BROADCAST_EVENT_H
#define BROADCAST_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef enum BroadcastEventType_t {
  EventTypeNone = 0,
  EventTypeAll,
  EventTypeGoingToSleep,
  EventTypeWakeUp,
  // power
  EventTypeChangeBatteryLevel,
  // screen
  EventTypeChangeScreenBrightness,
} BroadcastEventType_t;

/**
 * @brief bit mapping:
 * [0:7] - type
 * [8:15] - index
 * [16:31] - payload
 */
typedef union BroadcastEvent_t {
  struct {
    _u16 payload : 16;
    _u8 index : 8;
    _u8 type : 8;
  };
  _u32 value;
} BroadcastEvent_t;

#ifdef __cplusplus
}
#endif

#endif  // BROADCAST_EVENT_H