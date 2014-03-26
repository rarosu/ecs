#pragma once

// Redefine private and protected to public so that unit tests can watch private members.
// Very ugly :) Also note that this is not standard compliant.
#define private public
#define protected public

#include <ecs.h>

#undef private
#undef protected
