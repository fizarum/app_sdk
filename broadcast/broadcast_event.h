#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * @brief bit mapping:
 * [0:7] - type
 * [8:23] - payload
 * [24:31] - payload annex
 */
typedef union broadcast_event_t {
  struct {
    _u8 type : 8;
    _u16 payload : 16;
    _u8 payloadAnnex : 8;
  };
  _u32 value;
} broadcast_event_t;

#ifdef __cplusplus
}
#endif
