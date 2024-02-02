

#ifndef ERSAP_ENGINE_DATA_TYPE_HPP
#define ERSAP_ENGINE_DATA_TYPE_HPP

#include <ersap/serializer.hpp>

#include <memory>
#include <string>

namespace ersap {

/**
 * Defines a data type used by a {@link Engine service engine}.
 * Data type can be a predefined type, or a custom-defined type.
 * When declaring a custom type, its serialization routine must be provided.
 */
class EngineDataType final
{
public:
    /**
     * Creates a new user data type.
     * The data type is identified by its mime-type string.
     * The serializer will be used in order to send data through the network,
     * or to a different language DPE.
     *
     * @param mimeType the name of this data-type
     * @param serializer the custom serializer for this data-type
     */
    EngineDataType(std::string mime_type,
                   std::unique_ptr<Serializer> serializer)
      : mime_type_{std::move(mime_type)}
      , serializer_{std::move(serializer)}
    {
        // nothing
    }

    /**
     * Returns the name of this data type.
     */
    const std::string& mime_type() const
    {
        return mime_type_;
    }

    /**
     * Returns the serializer of this data type.
     */
    const Serializer* serializer() const
    {
        return serializer_.get();
    }

private:
    std::string mime_type_;
    std::shared_ptr<Serializer> serializer_;
};


inline
bool operator==(const EngineDataType& data_type, const std::string& mime_type)
{
    return data_type.mime_type() == mime_type;
}

inline
bool operator==(const std::string& mime_type, const EngineDataType& data_type)
{
    return data_type == mime_type;
}


inline
bool operator!=(const EngineDataType& data_type, const char* mime_type)
{
    return !(data_type == mime_type);
}

inline
bool operator!=(const std::string& mime_type, const EngineDataType& data_type)
{
    return !(data_type == mime_type);
}


/**
 * Predefined ERSAP data types.
 */
namespace type {

/**
 * Signed int of 32 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType SINT32;

/**
 * Signed int of 64 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType SINT64;

/**
 * Signed fixed integer of 32 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType SFIXED32;

/**
 * Signed fixed integer of 64 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType SFIXED64;

/**
 * A float (32 bits floating-point number).
 */
extern const EngineDataType FLOAT;

/**
 * A double (64 bits floating-point number).
 */
extern const EngineDataType DOUBLE;

/**
 * A string.
 */
extern const EngineDataType STRING;

/**
 * Raw bytes.
 * On Java a {@link ByteBuffer} is used to wrap the byte array and its endianess.
 */
extern const EngineDataType BYTES;

/**
 * An array of signed varints of 32 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType ARRAY_SINT32;

/**
 * An array of signed varints of 64 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType ARRAY_SINT64;

/**
 * An array of signed fixed integers of 32 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType ARRAY_SFIXED32;

/**
 * An array of signed fixed integers of 64 bits.
 *
 * @see <a href="https://developers.google.com/protocol-buffers/docs/encoding">Wire types</a>
 */
extern const EngineDataType ARRAY_SFIXED64;

/**
 * An array of floats (32 bits floating-point numbers).
 */
extern const EngineDataType ARRAY_FLOAT;

/**
 * An array of doubles (64 bits floating-point numbers).
 */
extern const EngineDataType ARRAY_DOUBLE;

/**
 * An array of strings.
 */
extern const EngineDataType ARRAY_STRING;

/**
 * JSON text.
 */
extern const EngineDataType JSON;

/**
 * An xMsg native data object.
 */
extern const EngineDataType NATIVE;

} // end namespace mime

} // end namespace ersap

#endif // end of include guard: ERSAP_ENGINE_DATA_TYPE_HPP
