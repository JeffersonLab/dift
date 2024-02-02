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

#ifndef XMSG_PROTO_META_H_
#define XMSG_PROTO_META_H_

#include "meta.pb.h"

#include <memory>
#include <stdexcept>

namespace xmsg {

/**
 * Protobuf data classes and helpers.
 * %xMsg uses [Protocol Buffers](https://developers.google.com/protocol-buffers)
 * for storing and sending metadata, simple data and registration information
 * between actors. The protobuf classes are defined in this namespace, along
 * with some helper functions that simplify creating objects of those classes
 * for common use cases.
 */
namespace proto {

/**
 * \class Meta
 * \brief Metadata for pub/sub communications.
 *
 * The fields in this protobuf class can be used to further describe
 * the message data in pub/sub communications.
 *
 * The most important is `datatype`, which identifies the type of the
 * serialized data. It is required for custom data objects, but it is set
 * automatically if the message is created with \ref make_message.
 *
 * Most other fields are optional or reserved.
 * `byteorder` must be set in case endiannes is important for serialization.
 * `communicationid` can be set to identify messages.
 * `replyto` is set by %xMsg to control sync communications.
 * The reserved fields like `status`, `composition`, `action` and `control`
 * should be set by higher-level layers.
 */

namespace detail {

inline void set_datatype(Meta& meta, const char* datatype)
{
    if (datatype != nullptr) {
        meta.set_datatype(datatype);
    } else {
        throw std::invalid_argument{"null mime-type"};
    }
}

inline void set_datatype(Meta& meta, const std::string& datatype)
{
    meta.set_datatype(datatype);
}

} // end namespace detail


/**
 * Creates an smart pointer to an empty %Meta object.
 */
inline std::unique_ptr<Meta> make_meta()
{
    return std::make_unique<Meta>();
}

/**
 * Creates an smart pointer to a copy of the given %Meta object.
 */
inline std::unique_ptr<Meta> copy_meta(const Meta& meta)
{
    return std::make_unique<Meta>(meta);
}


inline bool operator==(const Meta& lhs, const Meta& rhs)
{
    return lhs.SerializeAsString() == rhs.SerializeAsString();
}

inline bool operator!=(const Meta& lhs, const Meta& rhs)
{
    return !(lhs == rhs);
}

} // end namespace proto
} // end namespace xmsg

#endif // XMSG_PROTO_META_H_
