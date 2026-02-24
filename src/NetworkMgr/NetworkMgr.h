#ifndef _NETWORKMGR_H_
#define _NETWORKMGR_H_

#include <cstdint>
#include <stdexcept>
#include <string>

#include "../AppMain/UserSettings.h"
#include "../wl/wl.h"

// defines an interface for the Network manager.
//
// instances are then pushed into the AppMain via
// dependency injection, which probably is the cleanest way?
//
class NetworkMgr {
   public:
    virtual bool check_connection() const = 0;
    virtual WL::Collection get(WlSettings&) = 0;
};

#endif
