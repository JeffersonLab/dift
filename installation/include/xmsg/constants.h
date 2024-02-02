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

#ifndef XMSG_CORE_CONSTANTS_H_
#define XMSG_CORE_CONSTANTS_H_

#include <string>

namespace xmsg {

/**
 * Global constants.
 */
namespace constants {

const std::string undefined = "undefined";
const std::string success = "success";
const std::string any = "*";

/// \cond HIDDEN_SYMBOLS
const std::string registrar = "xMsg_Registrar";

const std::string register_publisher = "registerPublisher";
const std::string register_subscriber = "registerSubscriber";
const int register_request_timeout = 3000;

const std::string remove_publisher = "removePublisherRegistration";
const std::string remove_subscriber = "removeSubscriberRegistration";
const std::string remove_all_registration = "removeAllRegistration";
const int remove_request_timeout = 3000;

const std::string find_publisher = "findPublisher";
const std::string find_subscriber = "findSubscriber";
const int find_request_timeout = 3000;
/// \endcond

const std::string info = "INFO";
const std::string warning = "WARNING";
const std::string error = "ERROR";
const std::string done = "done";
const std::string data = "data";

const std::string no_result = "none";

/// \cond HIDDEN_SYMBOLS
const std::string bind = "bind";
const std::string connect = "connect";

const std::string ctrl_topic = "xmsg:control";
const std::string ctrl_connect = "pub";
const std::string ctrl_subscribe = "sub";
const std::string ctrl_reply = "rep";
/// \endcond

// clang-format off
const int default_port = 7771;      ///< Default publication port for proxies
const int registrar_port = 8888;    ///< Default listening port for registrar services
// clang-format on

} // end namespace constants


/**
 * Identifiers for base data types.
 * Most of these types represent the values that can be stored in
 * proto::Data objects, and are set automatically
 * when a message is created with \ref make_message.
 * For more information about the Protocol Buffer value types, check
 * [here](https://developers.google.com/protocol-buffers/docs/proto#scalar).
 *
 * Clients must define their own strings for custom data types.
 * The identifier should be used to check that the message contains
 * the expected data.
 */
namespace mimetype {

// clang-format off
const std::string single_sint32 = "binary/sint32";          ///< Signed integer. Uses variable-length encoding.
const std::string single_sint64 = "binary/sint64";          ///< Signed integer. Uses variable-length encoding.
const std::string single_sfixed32 = "binary/sfixed32";      ///< Signed integer. Always use 4 bytes.
const std::string single_sfixed64 = "binary/sfixed64";      ///< Signed integer. Always use 8 bytes.
const std::string single_float = "binary/float";            ///< Single precision floating point type.
const std::string single_double = "binary/double";          ///< Double precision floating point type.
const std::string single_string = "text/string";            ///< An UTF-8 encoded or 7-bit ASCII text.
const std::string bytes = "binary/bytes";                   ///< An arbitrary sequence of bytes.

const std::string array_sint32 = "binary/array-sint32";     ///< Repeated signed integers. Uses variable-length encoding.
const std::string array_sint64 = "binary/array-sint64";     ///< Repeated signed integers. Uses variable-length encoding.
const std::string array_sfixed32 = "binary/array-sfixed32"; ///< Repeated signed integers. Always use 4 bytes.
const std::string array_sfixed64 = "binary/array-sfixed32"; ///< Repeated signed integers. Always use 8 bytes.
const std::string array_float = "binary/array-float";       ///< Repeated single precision floating point types.
const std::string array_double = "binary/array-double";     ///< Repeated double precision floating point types.
const std::string array_string = "binary/array-string";     ///< Repeated UTF-8 encoded or 7-bit ASCII texts.
const std::string array_bytes = "binary/array-bytes";       ///< Repeated arbitrary sequences of bytes.

const std::string xmsg_data = "binary/native";              ///< A serialized \ref xmsg::proto::Data "Data" object
const std::string java_object = "binary/java";              ///< A serialized Java object
const std::string cpp_object = "binary/cpp";                ///< A serialized C++ object
const std::string python_object = "binary/python";          ///< A serialized Python object
// clang-format on

} // end namespace mime

} // end namespace xmsg

#endif // XMSG_CORE_CONSTANTS_H_
