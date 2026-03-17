#ifndef NETWORKMGR_H
#define NETWORKMGR_H

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include "UserSettings.h"
#include "wl/wl.h"

// defines an interface for the Network manager.
//
// instances are then pushed into the AppMain via
// dependency injection, which probably is the cleanest way?
//
class NetworkMgr {
   public:
    virtual bool set_up(const WifiSettings&) = 0;
    virtual bool is_set_up() const = 0;
    virtual bool check_connection() const = 0;
    virtual std::shared_ptr<WL::Collection> get(
        std::shared_ptr<WlSettings>) = 0;
};

#endif
