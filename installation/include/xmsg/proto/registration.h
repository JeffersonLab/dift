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

#ifndef XMSG_PROTO_REG_H_
#define XMSG_PROTO_REG_H_

#include "registration.pb.h"

#include <xmsg/address.h>
#include <xmsg/topic.h>

#include <set>

namespace xmsg {

namespace proto {

/**
 * \class Registration
 * \brief Information of a registered pub or sub actor.
 *
 * Objects of this class describe a registered pub or sub xMsg actor.
 * The `name` identifies an actor that is publishing or subscribed to
 * (`ownertype`) messages
 * of the specified topic (`domain`, `subject` and `type`)
 * on the proxy address (`host` and `port`).
 */

/// \cond HIDDEN_SYMBOLS
struct CompareRegistration
{
    bool operator()(const Registration& lhs, const Registration& rhs) const;
};
/// \endcond


/**
 * Gets the topic from the given %Registration data.
 */
inline Topic parse_topic(const Registration& reg)
{
    return Topic::build(reg.domain(), reg.subject(), reg.type());
}

/**
 * Gets the address from the given %Registration data.
 */
inline ProxyAddress parse_address(const Registration& reg)
{
    return { reg.host(), reg.port() };
}

bool operator==(const Registration& lhs, const Registration& rhs);
bool operator!=(const Registration& lhs, const Registration& rhs);

} // end namespace proto

/**
 * The set of Registration objects that result of a discovery query.
 */
using RegDataSet = std::set<proto::Registration, proto::CompareRegistration>;

} // end namespace xmsg

#endif // XMSG_PROTO_REG_H_
