

#ifndef ERSAP_DATA_SERIALIZATION_H
#define ERSAP_DATA_SERIALIZATION_H

#include <ersap/any.hpp>

#include <vector>

namespace ersap {

class Serializer
{
public:
    /**
     * Serializes the user object into a byte buffer and returns it.
     *
     * @param data the user object stored on the {@link EngineData}
     * @throws ErsapException if the data could not be serialized
     */
    virtual std::vector<std::uint8_t> write(const any& data) const = 0;

    /**
     * De-serializes the byte buffer into the user object and returns it.
     *
     * @param buffer the serialized data
     * @throws ErsapException if the data could not be deserialized
     */
    virtual any read(const std::vector<std::uint8_t>& buffer) const = 0;

public:
    /**
     * Serializes the user object into a byte buffer and returns it.
     *
     * @param data the user object stored on the {@link EngineData}
     * @throws ErsapException if the data could not be serialized
     */
    virtual any read(std::vector<std::uint8_t>&& buffer) const
    {
        return read(buffer);
    }

    /**
     * De-serializes the byte buffer into the user object and returns it.
     *
     * @param buffer the serialized data
     * @throws ErsapException if the data could not be deserialized
     */
    virtual std::vector<std::uint8_t> write(any&& data) const
    {
        return write(data);
    }

public:
    virtual ~Serializer() = default;
};

} // end namespace ersap

#endif // end of include guard: ERSAP_DATA_SERIALIZATION_H
