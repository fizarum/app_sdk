#pragma once

#include "../app.h"

typedef struct broadcast_subsriber_t {
  broadcast_event_type_t type;
  app_specification_t* spec;
} broadcast_subsriber_t;