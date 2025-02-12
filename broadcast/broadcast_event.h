#ifndef BROADCAST_EVENT_H
#define BROADCAST_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef enum BroadcastEventType_t {
  EventTypeNone,
  EventTypeAll,
  EventTypeGoingToSleep,
  EventTypeWakeUp,

  // power
  ChargingOn,
  ChargingOff,

  // screen
  EventTypeChangeScreenBrightness,
} BroadcastEventType_t;

/**
 * @brief bit mapping:
 * [0:7] - type
 * [8:23] - payload
 * [24:31] - payload annex
 */
typedef union BroadcastEvent_t {
  struct {
    _u8 type : 8;
    _u16 payload : 16;
    _u8 payloadAnnex : 8;
  };
  _u32 value;
} BroadcastEvent_t;

#ifdef __cplusplus
}
#endif

#endif  // BROADCAST_EVENT_H