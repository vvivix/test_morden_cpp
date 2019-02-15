//
// Created by vvivi on 2018/12/10.
//
#include "gtest/gtest.h"

enum class TagType : uint32_t {
    INVALID = 0u, // (0 << 28)
    ENUM = 268435456u, // (1 << 28)
    ENUM_REP = 536870912u, // (2 << 28)
    UINT = 805306368u, // (3 << 28)
    UINT_REP = 1073741824u, // (4 << 28)
    ULONG = 1342177280u, // (5 << 28)
    DATE = 1610612736u, // (6 << 28)
    BOOL = 1879048192u, // (7 << 28)
    BIGNUM = 2147483648u, // (8 << 28)
    BYTES = 2415919104u, // (9 << 28)
    ULONG_REP = 2684354560u, // (10 << 28)
};


enum class Tag : uint32_t {
    INVALID = 0u, // (TagType:INVALID | 0)
    PURPOSE = 536870913u, // (TagType:ENUM_REP | 1)
    ALGORITHM = 268435458u, // (TagType:ENUM | 2)
    KEY_SIZE = 805306371u, // (TagType:UINT | 3)
    BLOCK_MODE = 536870916u, // (TagType:ENUM_REP | 4)
    DIGEST = 536870917u, // (TagType:ENUM_REP | 5)
    PADDING = 536870918u, // (TagType:ENUM_REP | 6)
    CALLER_NONCE = 1879048199u, // (TagType:BOOL | 7)
    MIN_MAC_LENGTH = 805306376u, // (TagType:UINT | 8)
    KDF = 536870921u, // (TagType:ENUM_REP | 9)
    EC_CURVE = 268435466u, // (TagType:ENUM | 10)
    RSA_PUBLIC_EXPONENT = 1342177480u, // (TagType:ULONG | 200)
    ECIES_SINGLE_HASH_MODE = 1879048393u, // (TagType:BOOL | 201)
    INCLUDE_UNIQUE_ID = 1879048394u, // (TagType:BOOL | 202)
    BLOB_USAGE_REQUIREMENTS = 268435757u, // (TagType:ENUM | 301)
    BOOTLOADER_ONLY = 1879048494u, // (TagType:BOOL | 302)
    ACTIVE_DATETIME = 1610613136u, // (TagType:DATE | 400)
    ORIGINATION_EXPIRE_DATETIME = 1610613137u, // (TagType:DATE | 401)
    USAGE_EXPIRE_DATETIME = 1610613138u, // (TagType:DATE | 402)
    MIN_SECONDS_BETWEEN_OPS = 805306771u, // (TagType:UINT | 403)
    MAX_USES_PER_BOOT = 805306772u, // (TagType:UINT | 404)
    ALL_USERS = 1879048692u, // (TagType:BOOL | 500)
    USER_ID = 805306869u, // (TagType:UINT | 501)
    USER_SECURE_ID = 2684355062u, // (TagType:ULONG_REP | 502)
    NO_AUTH_REQUIRED = 1879048695u, // (TagType:BOOL | 503)
    USER_AUTH_TYPE = 268435960u, // (TagType:ENUM | 504)
    AUTH_TIMEOUT = 805306873u, // (TagType:UINT | 505)
    ALLOW_WHILE_ON_BODY = 1879048698u, // (TagType:BOOL | 506)
    ALL_APPLICATIONS = 1879048792u, // (TagType:BOOL | 600)
    APPLICATION_ID = 2415919705u, // (TagType:BYTES | 601)
    EXPORTABLE = 1879048794u, // (TagType:BOOL | 602)
    APPLICATION_DATA = 2415919804u, // (TagType:BYTES | 700)
    CREATION_DATETIME = 1610613437u, // (TagType:DATE | 701)
    ORIGIN = 268436158u, // (TagType:ENUM | 702)
    ROLLBACK_RESISTANT = 1879048895u, // (TagType:BOOL | 703)
    ROOT_OF_TRUST = 2415919808u, // (TagType:BYTES | 704)
    OS_VERSION = 805307073u, // (TagType:UINT | 705)
    OS_PATCHLEVEL = 805307074u, // (TagType:UINT | 706)
    UNIQUE_ID = 2415919811u, // (TagType:BYTES | 707)
    ATTESTATION_CHALLENGE = 2415919812u, // (TagType:BYTES | 708)
    ATTESTATION_APPLICATION_ID = 2415919813u, // (TagType:BYTES | 709)
    ATTESTATION_ID_BRAND = 2415919814u, // (TagType:BYTES | 710)
    ATTESTATION_ID_DEVICE = 2415919815u, // (TagType:BYTES | 711)
    ATTESTATION_ID_PRODUCT = 2415919816u, // (TagType:BYTES | 712)
    ATTESTATION_ID_SERIAL = 2415919817u, // (TagType:BYTES | 713)
    ATTESTATION_ID_IMEI = 2415919818u, // (TagType:BYTES | 714)
    ATTESTATION_ID_MEID = 2415919819u, // (TagType:BYTES | 715)
    ATTESTATION_ID_MANUFACTURER = 2415919820u, // (TagType:BYTES | 716)
    ATTESTATION_ID_MODEL = 2415919821u, // (TagType:BYTES | 717)
    ASSOCIATED_DATA = 2415920104u, // (TagType:BYTES | 1000)
    NONCE = 2415920105u, // (TagType:BYTES | 1001)
    AUTH_TOKEN = 2415920106u, // (TagType:BYTES | 1002)
    MAC_LENGTH = 805307371u, // (TagType:UINT | 1003)
    RESET_SINCE_ID_ROTATION = 1879049196u, // (TagType:BOOL | 1004)
};

constexpr TagType typeFromTag(Tag tag) {
    return static_cast<TagType>(static_cast<uint32_t>(tag) & static_cast<uint32_t>(0xf0000000));
}


/**
 * TypedTag is a templatized version of Tag, which provides compile-time checking of
 * keymaster tag types. Instances are convertible to Tag, so they can be used wherever
 * Tag is expected, and because they encode the tag type it's possible to create
 * function overloads that only operate on tags with a particular type.
 */
template <TagType tag_type, Tag tag> struct TypedTag {
    inline TypedTag() {
        // Ensure that it's impossible to create a TypedTag instance whose 'tag' doesn't have type
        // 'tag_type'.  Attempting to instantiate a tag with the wrong type will result in a compile
        // error (no match for template specialization StaticAssert<false>), with no run-time cost.
        static_assert(typeFromTag(tag) == tag_type, "mismatch between tag and tag_type");
    }
    operator Tag() const { return tag; }
};


template <Tag tag> struct Tag2TypedTag { typedef TypedTag<typeFromTag(tag), tag> type; };

template <Tag tag> struct Tag2String;


#define _TAGS_STRINGIFY(x) #x
#define TAGS_STRINGIFY(x) _TAGS_STRINGIFY(x)

#define DECLARE_TYPED_TAG(name)                                                                    \
    typedef typename Tag2TypedTag<Tag::name>::type TAG_##name##_t;                                 \
    extern TAG_##name##_t TAG_##name;                                                              \
    template <> struct Tag2String<Tag::name> {                                                     \
        static const char* value() { return "Tag::" TAGS_STRINGIFY(name); }                        \
    }

DECLARE_TYPED_TAG(INVALID);
DECLARE_TYPED_TAG(KEY_SIZE);
DECLARE_TYPED_TAG(MAC_LENGTH);
DECLARE_TYPED_TAG(CALLER_NONCE);
DECLARE_TYPED_TAG(MIN_MAC_LENGTH);
DECLARE_TYPED_TAG(RSA_PUBLIC_EXPONENT);
DECLARE_TYPED_TAG(ECIES_SINGLE_HASH_MODE);
DECLARE_TYPED_TAG(INCLUDE_UNIQUE_ID);
DECLARE_TYPED_TAG(ACTIVE_DATETIME);
DECLARE_TYPED_TAG(ORIGINATION_EXPIRE_DATETIME);
DECLARE_TYPED_TAG(USAGE_EXPIRE_DATETIME);
DECLARE_TYPED_TAG(MIN_SECONDS_BETWEEN_OPS);
DECLARE_TYPED_TAG(MAX_USES_PER_BOOT);
DECLARE_TYPED_TAG(ALL_USERS);
DECLARE_TYPED_TAG(USER_ID);
DECLARE_TYPED_TAG(USER_SECURE_ID);
DECLARE_TYPED_TAG(NO_AUTH_REQUIRED);
DECLARE_TYPED_TAG(AUTH_TIMEOUT);
DECLARE_TYPED_TAG(ALLOW_WHILE_ON_BODY);
DECLARE_TYPED_TAG(ALL_APPLICATIONS);
DECLARE_TYPED_TAG(APPLICATION_ID);
DECLARE_TYPED_TAG(APPLICATION_DATA);
DECLARE_TYPED_TAG(CREATION_DATETIME);
DECLARE_TYPED_TAG(ROLLBACK_RESISTANT);
DECLARE_TYPED_TAG(ROOT_OF_TRUST);
DECLARE_TYPED_TAG(ASSOCIATED_DATA);
DECLARE_TYPED_TAG(NONCE);
DECLARE_TYPED_TAG(AUTH_TOKEN);
DECLARE_TYPED_TAG(BOOTLOADER_ONLY);
DECLARE_TYPED_TAG(OS_VERSION);
DECLARE_TYPED_TAG(OS_PATCHLEVEL);
DECLARE_TYPED_TAG(UNIQUE_ID);
DECLARE_TYPED_TAG(ATTESTATION_CHALLENGE);
DECLARE_TYPED_TAG(ATTESTATION_APPLICATION_ID);
DECLARE_TYPED_TAG(RESET_SINCE_ID_ROTATION);

DECLARE_TYPED_TAG(PURPOSE);
DECLARE_TYPED_TAG(ALGORITHM);
DECLARE_TYPED_TAG(BLOCK_MODE);
DECLARE_TYPED_TAG(DIGEST);
DECLARE_TYPED_TAG(PADDING);
DECLARE_TYPED_TAG(BLOB_USAGE_REQUIREMENTS);
DECLARE_TYPED_TAG(ORIGIN);
DECLARE_TYPED_TAG(USER_AUTH_TYPE);
DECLARE_TYPED_TAG(KDF);
DECLARE_TYPED_TAG(EC_CURVE);

template <typename... Elems> struct MetaList {};

using all_tags_t = MetaList<
        TAG_INVALID_t, TAG_KEY_SIZE_t, TAG_MAC_LENGTH_t, TAG_CALLER_NONCE_t, TAG_MIN_MAC_LENGTH_t,
        TAG_RSA_PUBLIC_EXPONENT_t, TAG_ECIES_SINGLE_HASH_MODE_t, TAG_INCLUDE_UNIQUE_ID_t,
        TAG_ACTIVE_DATETIME_t, TAG_ORIGINATION_EXPIRE_DATETIME_t, TAG_USAGE_EXPIRE_DATETIME_t,
        TAG_MIN_SECONDS_BETWEEN_OPS_t, TAG_MAX_USES_PER_BOOT_t, TAG_ALL_USERS_t, TAG_USER_ID_t,
        TAG_USER_SECURE_ID_t, TAG_NO_AUTH_REQUIRED_t, TAG_AUTH_TIMEOUT_t, TAG_ALLOW_WHILE_ON_BODY_t,
        TAG_ALL_APPLICATIONS_t, TAG_APPLICATION_ID_t, TAG_APPLICATION_DATA_t, TAG_CREATION_DATETIME_t,
        TAG_ROLLBACK_RESISTANT_t, TAG_ROOT_OF_TRUST_t, TAG_ASSOCIATED_DATA_t, TAG_NONCE_t,
        TAG_AUTH_TOKEN_t, TAG_BOOTLOADER_ONLY_t, TAG_OS_VERSION_t, TAG_OS_PATCHLEVEL_t, TAG_UNIQUE_ID_t,
        TAG_ATTESTATION_CHALLENGE_t, TAG_ATTESTATION_APPLICATION_ID_t, TAG_RESET_SINCE_ID_ROTATION_t,
        TAG_PURPOSE_t, TAG_ALGORITHM_t, TAG_BLOCK_MODE_t, TAG_DIGEST_t, TAG_PADDING_t,
        TAG_BLOB_USAGE_REQUIREMENTS_t, TAG_ORIGIN_t, TAG_USER_AUTH_TYPE_t, TAG_KDF_t, TAG_EC_CURVE_t>;

namespace keystore {

    template<typename TagList>
    struct TagStringifier;

    template<typename ... Tags>
    struct TagStringifier<MetaList<Tags...>> {
        template<TagType tag_type, Tag tag>
        static TypedTag<tag_type, tag> chooseString(TypedTag<tag_type, tag> ttag, Tag runtime_tag,
                                                    const char** result) {
            if (tag == runtime_tag) {
                *result = Tag2String<tag>::value();
            }
            return ttag;
        }
        static const char* stringify(Tag tag) {
            const char* result = "unknown tag";
            [] (Tags&&...) {}(chooseString(Tags(), tag, &result)...);
            return result;
        }
    };

const char* stringifyTag(Tag tag) {
    return TagStringifier<all_tags_t>::stringify(tag);
}

}

TEST(KeyStoreTagUtils, Case1) {

    std::cout << keystore::stringifyTag(Tag::INVALID) << std::endl;
    std::cout << keystore::stringifyTag(Tag::KEY_SIZE) << std::endl;
}
