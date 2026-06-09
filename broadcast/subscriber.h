#pragma once

#include "event_type.h"

typedef struct broadcast_subsriber_t {
  broadcast_event_type_t type;
  _u16 subscriber_id;
} broadcast_subsriber_t;