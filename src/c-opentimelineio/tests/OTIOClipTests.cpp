#include "gtest/gtest.h"

#include <copentime/rationalTime.h>
#include <copentime/timeRange.h>
#include <copentimelineio/clip.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/externalReference.h>
#include <copentimelineio/item.h>
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/missingReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>

class OTIOClipTests : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(OTIOClipTests, ConstructorTest)
{
    const char*   name = "test";
    RationalTime* rt   = RationalTime_create(5, 24);
    TimeRange*    tr   = TimeRange_create_with_start_time_and_duration(rt, rt);
    RationalTime* dur  = RationalTime_create(10, 24);
    TimeRange*    available_range =
        TimeRange_create_with_start_time_and_duration(rt, dur);
    ExternalReference* mr =
        ExternalReference_create("/var/tmp/test.mov", available_range, NULL);
    Clip* clip = Clip_create(name, (MediaReference*) mr, tr, NULL);
    EXPECT_STREQ(
        name,
        SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata*) clip));

    TimeRange* source_range = Item_source_range((Item*) clip);
    EXPECT_TRUE(TimeRange_equal(source_range, tr));

    MediaReference* media_reference = Clip_media_reference(clip);
    EXPECT_TRUE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(media_reference),
        reinterpret_cast<OTIOSerializableObject*>(mr)));

    Any* clip_any =
        create_safely_typed_any_serializable_object(reinterpret_cast<OTIOSerializableObject*>(clip));
    OTIOErrorStatus* errorStatus = OTIOErrorStatus_create();

    const char* encoded = serialize_json_to_string(clip_any, errorStatus, 4);
    Any*        decoded = /* allocate memory for destinantion */
        create_safely_typed_any_serializable_object(reinterpret_cast<OTIOSerializableObject*>(clip));

    /*bool decoded_successfully = */
        deserialize_json_from_string(encoded, decoded, errorStatus);
    OTIOSerializableObject* decoded_object = safely_cast_retainer_any(decoded);

    EXPECT_TRUE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(clip), decoded_object));

    RationalTime_destroy(rt);
    rt = NULL;
    TimeRange_destroy(tr);
    tr = NULL;
    RationalTime_destroy(dur);
    dur = NULL;
    TimeRange_destroy(available_range);
    available_range = NULL;
    TimeRange_destroy(source_range);
    source_range = NULL;
    Any_destroy(clip_any);
    clip_any = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
}

TEST_F(OTIOClipTests, RangesTest)
{
    const char*   name     = "test_clip";
    RationalTime* start    = RationalTime_create(86400, 24);
    RationalTime* duration = RationalTime_create(200, 24);
    TimeRange*    tr =
        TimeRange_create_with_start_time_and_duration(start, duration);
    ExternalReference* mr =
        ExternalReference_create("/var/tmp/test.mov", tr, NULL);
    Clip*            clip = Clip_create(name, (MediaReference*) mr, tr, NULL);
    RetainerSerializableObject* clip_r = RetainerSerializableObject_create(reinterpret_cast<OTIOSerializableObject*>(clip));
    OTIOErrorStatus* errorStatus = OTIOErrorStatus_create();

    RationalTime* clip_duration = Item_duration((Item*) clip, errorStatus);
    TimeRange*    clip_trimmed_range =
        Item_trimmed_range((Item*) clip, errorStatus);
    RationalTime* clip_trimmed_range_duration =
        TimeRange_duration(clip_trimmed_range);

    EXPECT_TRUE(RationalTime_equal(clip_duration, clip_trimmed_range_duration));

    RationalTime* tr_duration = TimeRange_duration(tr);
    EXPECT_TRUE(RationalTime_equal(clip_duration, tr_duration));
    EXPECT_TRUE(TimeRange_equal(tr, clip_trimmed_range));

    TimeRange* clip_available_range = Clip_available_range(clip, errorStatus);
    EXPECT_TRUE(TimeRange_equal(tr, clip_available_range));

    RationalTime_destroy(start);
    start = NULL;
    RationalTime_destroy(duration);
    duration = NULL;
    RationalTime_destroy(clip_duration);
    clip_duration = NULL;
    TimeRange_destroy(clip_trimmed_range);
    clip_trimmed_range = NULL;
    RationalTime_destroy(clip_trimmed_range_duration);
    clip_trimmed_range_duration = NULL;
    TimeRange_destroy(clip_available_range);
    clip_available_range = NULL;

    start    = RationalTime_create(86500, 24);
    duration = RationalTime_create(50, 24);
    TimeRange* source_range =
        TimeRange_create_with_start_time_and_duration(start, duration);
    Item_set_source_range((Item*) clip, source_range);
    clip_duration = Item_duration((Item*) clip, errorStatus);
    EXPECT_FALSE(RationalTime_equal(clip_duration, tr_duration));

    clip_trimmed_range = Item_trimmed_range((Item*) clip, errorStatus);
    EXPECT_FALSE(TimeRange_equal(clip_trimmed_range, tr));

    TimeRange*    clip_source_range = Item_source_range((Item*) clip);
    RationalTime* clip_source_range_duration =
        TimeRange_duration(clip_source_range);
    EXPECT_TRUE(RationalTime_equal(clip_source_range_duration, clip_duration));
    EXPECT_TRUE(TimeRange_equal(clip_trimmed_range, clip_source_range));

    TimeRange_destroy(tr);
    tr = NULL;
    RationalTime_destroy(tr_duration);
    tr_duration = NULL;
    RetainerSerializableObject_managed_destroy(clip_r);
    clip = NULL;
    RationalTime_destroy(start);
    start = NULL;
    RationalTime_destroy(duration);
    duration = NULL;
    TimeRange_destroy(source_range);
    source_range = NULL;
    RationalTime_destroy(clip_duration);
    clip_duration = NULL;
    TimeRange_destroy(clip_trimmed_range);
    clip_trimmed_range = NULL;
    TimeRange_destroy(clip_source_range);
    clip_source_range = NULL;
    RationalTime_destroy(clip_source_range_duration);
    clip_source_range_duration = NULL;
}

TEST_F(OTIOClipTests, RefDefaultTest)
{
    Clip*             clip = Clip_create(NULL, NULL, NULL, NULL);
    RetainerSerializableObject* clip_r = RetainerSerializableObject_create(reinterpret_cast<OTIOSerializableObject*>(clip));
    MissingReference* missing_reference = MissingReference_create(NULL, NULL, NULL);
    RetainerSerializableObject* missing_reference_r = RetainerSerializableObject_create(reinterpret_cast<OTIOSerializableObject*>(missing_reference));
    MediaReference* clip_media_reference = Clip_media_reference(clip);
    RetainerSerializableObject* clip_media_reference_r = RetainerSerializableObject_create(reinterpret_cast<OTIOSerializableObject*>(clip_media_reference));
    EXPECT_TRUE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(missing_reference),
        reinterpret_cast<OTIOSerializableObject*>(clip_media_reference)));

    RetainerSerializableObject_managed_destroy(missing_reference_r);
    clip_media_reference = NULL;

    ExternalReference* external_reference = ExternalReference_create(NULL, NULL, NULL);
    RetainerSerializableObject* external_reference_r = RetainerSerializableObject_create(reinterpret_cast<OTIOSerializableObject*>(external_reference));
    Clip_set_media_reference(clip, (MediaReference*) external_reference);
    clip_media_reference = Clip_media_reference(clip);
    EXPECT_TRUE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(external_reference),
        reinterpret_cast<OTIOSerializableObject*>(clip_media_reference)));

    RetainerSerializableObject_managed_destroy(clip_r);
    RetainerSerializableObject_managed_destroy(clip_media_reference_r);
    RetainerSerializableObject_managed_destroy(external_reference_r);
    clip = NULL;
}
