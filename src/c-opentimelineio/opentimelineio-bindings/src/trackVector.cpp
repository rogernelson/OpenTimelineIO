#include "copentimelineio/trackVector.h"
#include <opentimelineio/track.h>
#include <vector>

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

OTIO_API TrackVector* TrackVector_create()
{
    return reinterpret_cast<TrackVector*>(new std::vector<otio::Track*>());
}
OTIO_API void TrackVector_destroy(TrackVector* self)
{
    if (!self)
        return;
    
    delete reinterpret_cast<std::vector<otio::Track*>*>(self);
}
OTIO_API TrackVectorIterator* TrackVector_begin(TrackVector* self)
{
    if (!self)
        return NULL;

    auto v = reinterpret_cast<std::vector<otio::Track*>*>(self);
    auto iter = v->begin();
    return reinterpret_cast<TrackVectorIterator*>(new decltype(iter)(iter));
}
OTIO_API TrackVectorIterator* TrackVector_end(TrackVector* self)
{
    if (!self)
        return NULL;

    auto v = reinterpret_cast<std::vector<otio::Track*>*>(self);
    auto iter = v->end();
    return reinterpret_cast<TrackVectorIterator*>(new decltype(iter)(iter));
}
OTIO_API int TrackVector_size(TrackVector* self)
{
    if (!self)
        return 0;

    return reinterpret_cast<std::vector<otio::Track*>*>(self)->size();
}
OTIO_API int TrackVector_max_size(TrackVector* self)
{
    if (!self)
        return 0;

    return reinterpret_cast<std::vector<otio::Track*>*>(self)->max_size();
}
OTIO_API int TrackVector_capacity(TrackVector* self)
{
    if (!self)
        return 0;

    return reinterpret_cast<std::vector<otio::Track*>*>(self)->capacity();
}
OTIO_API void TrackVector_resize(TrackVector* self, int n)
{
    if (!self)
        return;

    reinterpret_cast<std::vector<otio::Track*>*>(self)->resize(n);
}
OTIO_API bool TrackVector_empty(TrackVector* self)
{
    if (!self)
        return true;
    
    return reinterpret_cast<std::vector<otio::Track*>*>(self)->empty();
}
OTIO_API void TrackVector_shrink_to_fit(TrackVector* self)
{
    if (!self)
        return;

    reinterpret_cast<std::vector<otio::Track*>*>(self)->shrink_to_fit();
}
OTIO_API void TrackVector_reserve(TrackVector* self, int n)
{
    if (!self)
        return;
    
    reinterpret_cast<std::vector<otio::Track*>*>(self)->reserve(n);
}
OTIO_API void TrackVector_swap(TrackVector* self, TrackVector* other)
{
    if (!self)
        return;
    
    reinterpret_cast<std::vector<otio::Track*>*>(self)->swap(
                                    *reinterpret_cast<std::vector<otio::Track*>*>(other));
}
OTIO_API Track* TrackVector_at(TrackVector* self, int pos)
{
    if (!self)
        return NULL;
    
    return reinterpret_cast<Track*>(
        reinterpret_cast<std::vector<otio::Track*>*>(self)->at(pos));
}
OTIO_API void TrackVector_push_back(TrackVector* self, Track* value)
{
    if (!self)
        return;
    
    reinterpret_cast<std::vector<otio::Track*>*>(self)->push_back(
        reinterpret_cast<otio::Track*>(value));
}
OTIO_API void TrackVector_pop_back(TrackVector* self)
{
    if (!self)
        return;
    
    reinterpret_cast<std::vector<otio::Track*>*>(self)->pop_back();
}
OTIO_API TrackVectorIterator*
TrackVector_insert(TrackVector* self, TrackVectorIterator* pos, Track* val)
{
    if (!self || !pos || !val)
        return NULL;

    auto v = reinterpret_cast<std::vector<otio::Track*>*>(self);
    auto p = reinterpret_cast<std::vector<otio::Track*>::iterator*>(pos);
    auto iter = v->insert(*p, reinterpret_cast<otio::Track*>(val));
    return reinterpret_cast<TrackVectorIterator*>(new decltype(iter)(iter));
}
OTIO_API void TrackVector_clear(TrackVector* self)
{
    if (!self)
        return;
    
    reinterpret_cast<std::vector<otio::Track*>*>(self)->clear();
}
OTIO_API TrackVectorIterator*
TrackVector_erase(TrackVector* self, TrackVectorIterator* pos)
{
    if (!self || !pos)
        return NULL;
    
    auto iter = reinterpret_cast<std::vector<otio::Track*>*>(self)->erase(
                    *reinterpret_cast<std::vector<otio::Track*>::iterator*>(pos));
    return reinterpret_cast<TrackVectorIterator*>(new decltype(iter)(iter));
}
OTIO_API TrackVectorIterator* TrackVector_erase_range(
    TrackVector*         self,
    TrackVectorIterator* first,
    TrackVectorIterator* last)
{
    if (!self || !first || !last)
        return 0;
    
    auto v = reinterpret_cast<std::vector<otio::Track*>*>(self);
    auto iter = v->erase(
            *reinterpret_cast<std::vector<otio::Track*>::iterator*>(first),
            *reinterpret_cast<std::vector<otio::Track*>::iterator*>(last));
    return reinterpret_cast<TrackVectorIterator*>(new decltype(iter)(iter));
}
OTIO_API void TrackVectorIterator_advance(TrackVectorIterator* iter, int dist)
{
    if (!iter)
        return;

    std::advance(*reinterpret_cast<std::vector<otio::Track*>::iterator*>(iter), dist);
}
OTIO_API Track* TrackVectorIterator_value(TrackVectorIterator* iter)
{
    if (!iter)
        return NULL;
    
    auto it = reinterpret_cast<std::vector<otio::Track*>::iterator*>(iter);
    otio::Track* obj = **it;
    return reinterpret_cast<Track*>(obj);
}
OTIO_API bool TrackVectorIterator_equal(
    TrackVectorIterator* lhs, TrackVectorIterator* rhs)
{
    if (!lhs || !rhs)
        return NULL;

    return *reinterpret_cast<std::vector<otio::Track*>::iterator*>(lhs) ==
            *reinterpret_cast<std::vector<otio::Track*>::iterator*>(rhs);
}
OTIO_API bool TrackVectorIterator_not_equal(
    TrackVectorIterator* lhs, TrackVectorIterator* rhs)
{
    if (!lhs || !rhs)
        return NULL;

    return *reinterpret_cast<std::vector<otio::Track*>::iterator*>(lhs) !=
            *reinterpret_cast<std::vector<otio::Track*>::iterator*>(rhs);
}
OTIO_API void TrackVectorIterator_destroy(TrackVectorIterator* self)
{
    if (!self)
        return;

    delete reinterpret_cast<std::vector<otio::Track*>::iterator*>(self);
}
