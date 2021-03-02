#pragma once

#include "opentimelineio/version.h"
#include "opentimelineio/serializableObjectWithMetadata.h"
#include "opentimelineio/bounds.h"

namespace opentimelineio { namespace OPENTIMELINEIO_VERSION  {

    using namespace opentime;
    
class MediaReference : public SerializableObjectWithMetadata {
public:
    struct Schema {
        static auto constexpr name = "MediaReference";
        static int constexpr version = 1;
    };

    using Parent = SerializableObjectWithMetadata;

    MediaReference(std::string const& name = std::string(),
                   optional<TimeRange> const& available_range = nullopt,
                   AnyDictionary const& metadata = AnyDictionary(),
                   Bounds* bounds = nullptr);

    optional<TimeRange> const& available_range () const {
        return _available_range;
    }

    void set_available_range(optional<TimeRange> const& available_range) {
        _available_range = available_range;
    }

    virtual bool is_missing_reference() const;
   
    Retainer<Bounds> bounds() const {
        return _bounds;
    }

    void set_bounds(Bounds* bounds) {
        _bounds = bounds;
    } 

protected:
    virtual ~MediaReference();

    virtual bool read_from(Reader&);
    virtual void write_to(Writer&) const;

private:
    optional<TimeRange> _available_range;
    Retainer<Bounds> _bounds;
};

} }
