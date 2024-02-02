/*
 * Copyright (C) 2015. Jefferson Lab, xMsg framework (JLAB). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and not-for-profit purposes,
 * without fee and without a signed licensing agreement.
 *
 * Contact Vardan Gyurjyan
 * Department of Experimental Nuclear Physics, Jefferson Lab.
 *
 * IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF
 * THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE CLARA SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 * HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef XMSG_CORE_MESSAGE_HPP_
#define XMSG_CORE_MESSAGE_HPP_

#include <xmsg/proto/data.h>
#include <xmsg/proto/meta.h>
#include <xmsg/topic.h>

#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>


namespace xmsg {

class xMsg;

/**
 * The standard message for %xMsg pub/sub communications.
 *
 * Messages are composed of a topic, metadata and data.
 *
 * %xMsg actors send (receive) messages to (from) other actors, using the
 * **topic** to identify and filter messages of interest.
 * See the Topic class documentation for details about topic matching.
 *
 * The **data** is always a byte buffer containing the serialized representation
 * of the value or object to be sent. For simple data types, the
 * proto::Data class can be used to store and serialize data, and the
 * \ref make_message helper should be preferred to create messages.
 * Otherwise it is up to the client to define the serialization of custom
 * complex objects.
 * The created message will give access to the binary buffer, which should
 * be deserialized to get the data back. With simple types,
 * \ref parse_message should be sufficient.
 *
 * The **metadata** can be used to provide further description of the data.
 * The `datatype` field is mandatory to identify the type of the data,
 * and can be used by clients to check if the message contains data they can
 * work with. It is responsibility of the publisher to create messages with
 * proper data and datatype (i.e. the datatype matches the data).
 * See proto::Meta for all available fields.
 *
 * When an actor publishes a message and expects a response
 * (using xMsg::sync_publish), the message will have the metadata field
 * `replyto` set to the topic where the response should be published.
 * To reuse the message for the response, construct it with \ref make_response.
 * Otherwise, create a new message using the \ref Message::replyto "replyto"
 * as the topic.
 */
class Message final
{
public:
    /**
     * Creates a new message with the given topic, metadata and serialized
     * data.
     *
     * The metadata must contain the correct data type identifier for the data,
     * and any other field required by the client.
     *
     * \tparam T Topic
     * \tparam V `std::vector<std::uint8_t>`
     * \param topic the topic of the message
     * \param metadata description of the data
     * \param data serialized user data
     */
    template<typename T, typename V>
    Message(T&& topic, std::unique_ptr<proto::Meta>&& metadata, V&& data)
      : topic_{std::forward<T>(topic)}
      , meta_{std::move(metadata)}
      , data_{std::forward<V>(data)}
    {
        if (!meta_) {
            throw std::invalid_argument{"null metadata"};
        }
    }

    /**
     * Creates a new message with the given topic, data type and serialized
     * data.
     *
     * The data type must be the correct identifier for the data.
     * The metadata will be automatically created with only the data type set.
     *
     * \tparam T Topic
     * \tparam S `std::string` or `const char*`
     * \tparam V `std::vector<std::uint8_t>`
     * \param topic the topic of the message
     * \param mimetype the (literal) string identifier of the data
     * \param data serialized user data
     */
    template<typename T, typename S, typename V>
    Message(T&& topic, S&& mimetype, V&& data)
      : topic_{std::forward<T>(topic)}
      , meta_{proto::make_meta()}
      , data_{std::forward<V>(data)}
    {
        proto::detail::set_datatype(*meta_, mimetype);
    }

    Message(const Message& other)
      : topic_{other.topic_}
      , meta_{proto::copy_meta(*other.meta_)}
      , data_{other.data_}
    { }

    Message& operator=(const Message& other)
    {
        topic_ = other.topic_;
        meta_ = proto::copy_meta(*other.meta_);
        data_ = other.data_;
        return *this;
    }

    Message(Message&&) = default;
    Message& operator=(Message&&) = default;

    ~Message() = default;

public:
    friend void swap(Message& lhs, Message& rhs)
    {
        using std::swap;
        swap(lhs.topic_, rhs.topic_);
        swap(lhs.meta_, rhs.meta_);
        swap(lhs.data_, rhs.data_);
    }

public:
    /// Read-only access to the topic
    const Topic& topic() const { return topic_; }

    /// Read-only access to the metadata
    const proto::Meta* meta() const { return meta_.get(); }

    /// Read-only access to the serialized data
    const std::vector<std::uint8_t>& data() const { return data_; }

public:
    /// Gets the `datatype` identifier from the metadata.
    const std::string& datatype() const { return meta_->datatype(); }

    /// Checks if the metadata contains a `replyto` value
    bool has_replyto() const { return meta_->has_replyto(); }

    /// Gets a %Topic using the `replyto` value from the metadata.
    /// Useful when creating a response message.
    Topic replyto() const { return Topic::raw(meta_->replyto()); }

public:
    friend Message make_response(Message&& msg);
    friend Message make_response(const Message& msg);

private:
    friend xMsg;
    Topic topic_;
    std::unique_ptr<proto::Meta> meta_;
    std::vector<std::uint8_t> data_;
};


/**
 * Creates a simple message with a data value of type D.
 *
 * D must be one of the types that can be set on proto::Data objects.
 * The meta field `datatype` will be set accordingly.
 * Use \ref parse_message(const Message&) "parse_message" to get the value back.
 *
 * This is just a one line wrapper to create a message using
 * proto::make_data and proto::serialize_data.
 * Call the same functions and the message constructor directly
 * if more setup is needed with the metadata field,
 * and `datatype` must be set carefully.
 *
 * \tparam T Topic
 * \tparam D a type that can be set on proto::Data objects
 * \param topic the topic of the message
 * \param data the value to be set in the message
 */
template<typename T,
         typename D,
         typename = std::enable_if_t<
            !std::is_same<proto::Data, std::decay_t<D>>::value
         >
        >
inline Message make_message(T&& topic, D&& data)
{
    auto xdata = proto::make_data(std::forward<D>(data));
    return {std::forward<T>(topic),
            proto::detail::get_mimetype<std::decay_t<D>>(),
            proto::serialize_data(xdata)};
}


/**
 * Creates a message with data of type proto::Data.
 * The meta field `datatype` will be set to mimetype::xmsg_data,
 * and the protobuf data will be serialized.
 * Use \ref parse_message to deserialized the object back.
 *
 * This is just a one line wrapper to create a message using
 * proto::serialize_data.
 * Call the same function and the message constructor directly
 * if more setup is needed with the metadata field,
 * and `datatype` must be set carefully.
 *
 * \tparam T Topic
 * \param topic the topic of the message
 * \param data the object to be serialized in the message
 */
template<typename T>
inline Message make_message(T&& topic, const proto::Data& data)
{
    auto buffer = proto::serialize_data(data);
    return {std::forward<T>(topic), mimetype::xmsg_data, std::move(buffer)};
}


/**
 * Deserializes a data of type T from the given message.
 *
 * \tparam T a type that can be get from proto::Data objects
 * \param msg the message
 * \return the value of type T contained in the message
 */
template<typename T>
inline T parse_message(const Message& msg)
{
    auto xdata = proto::parse_data(msg.data());
    return proto::parse_data<T>(xdata);
}


/**
 * Deserializes a proto::Data object from the given message.
 *
 * \param msg the message
 * \return the deserialized %Data object
 */
template<>
inline proto::Data parse_message(const Message& msg)
{
    return proto::parse_data(msg.data());
}


/**
 * Moves the given message into a response message.
 *
 * - The topic will be set to \ref Message::replyto "msg.replyto()"
 * - The metadata will be reused (and the `replyto` field will be cleared)
 * - The data will be reused
 */
inline Message make_response(Message&& msg)
{
    msg.topic_ = msg.replyto();
    msg.meta_->clear_replyto();
    return std::move(msg);
}


/**
 * Copies the given message into a response message.
 *
 * - The topic will be set to \ref Message::replyto "msg.replyto()"
 * - The metadata will be copied (and the `replyto` field will be cleared)
 * - The data will be copied
 */
inline Message make_response(const Message& msg)
{
    auto meta = proto::copy_meta(*msg.meta_);
    meta->clear_replyto();
    return {msg.replyto(), std::move(meta), msg.data_};
}


inline bool operator==(const Message& lhs, const Message& rhs)
{
    return std::tie(lhs.topic().str(), *lhs.meta(), lhs.data())
        == std::tie(rhs.topic().str(), *rhs.meta(), rhs.data());
}

inline bool operator!=(const Message& lhs, const Message& rhs)
{
    return !(lhs == rhs);
}


} // end namespace xmsg

#endif // XMSG_CORE_MESSAGE_H_
