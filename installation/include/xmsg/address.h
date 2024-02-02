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

#ifndef XMSG_CORE_ADDRESS_H_
#define XMSG_CORE_ADDRESS_H_

#include <functional>
#include <string>
#include <tuple>

namespace xmsg {

/**
 * The network address of an %xMsg proxy.
 * Proxies connect and dispatch messages between %xMsg actors running on
 * a cloud of nodes. By default, proxies use localhost IP as its address,
 * the \ref constants::default_port as the PUB port, and PUB+1 as the SUB
 * port.
 */
class ProxyAddress final
{
public:
    /// Creates an address using localhost and default ports
    ProxyAddress();

    /// Creates an address using provided host and default ports
    explicit ProxyAddress(const std::string& host);

    /// Creates an address using provided host and PUB port,
    /// with default SUB port
    ProxyAddress(const std::string& host, int pub_port);

public:
    /// The host IP of the proxy
    const std::string& host() const { return host_; }

    /// The publication port of the proxy
    int pub_port() const { return pub_port_; }

    /// The subscription port of the proxy
    int sub_port() const { return sub_port_; }

private:
    std::string host_;
    int pub_port_;
    int sub_port_;

    friend bool operator==(const ProxyAddress& lhs, const ProxyAddress& rhs);
};


/**
 * The network address of an %xMsg registrar service.
 * Registration services allow discoverability of running %xMsg actors.
 * By default, registrar services use localhost IP as its address,
 * and \ref constants::registrar_port as the listening port.
 */
class RegAddress final
{
public:
    /// Creates an address using localhost and default port
    RegAddress();

    /// Creates an address using provided host and default port
    explicit RegAddress(const std::string& host);

    /// Creates an address using provided host and port
    RegAddress(const std::string& host, int port);

public:
    /// The host IP of the registrar service
    const std::string& host() const { return host_; }

    /// The listening port of the registrar service
    int port() const { return port_; }

private:
    std::string host_;
    int port_;

    friend bool operator==(const RegAddress& lhs, const RegAddress& rhs);
};


inline bool operator==(const ProxyAddress& lhs, const ProxyAddress& rhs)
{
    return std::tie(lhs.host_, lhs.pub_port_, lhs.sub_port_)
        == std::tie(rhs.host_, rhs.pub_port_, rhs.sub_port_);
}

inline bool operator!=(const ProxyAddress& lhs, const ProxyAddress& rhs)
{
    return !(lhs == rhs);
}


inline bool operator==(const RegAddress& lhs, const RegAddress& rhs)
{
    return std::tie(lhs.host_, lhs.port_) == std::tie(rhs.host_, rhs.port_);
}

inline bool operator!=(const RegAddress& lhs, const RegAddress& rhs)
{
    return !(lhs == rhs);
}


std::ostream& operator<<(std::ostream& os, const ProxyAddress& a);

std::ostream& operator<<(std::ostream& os, const RegAddress& a);

} // end namespace xmsg


namespace std {

template <>
struct hash<xmsg::ProxyAddress>
{
    std::size_t operator()(const xmsg::ProxyAddress& k) const
    {
        using std::hash;
        using std::string;

        return hash<std::string>()(k.host()) ^ (hash<int>()(k.pub_port()) << 1);
    }
};

template <>
struct hash<xmsg::RegAddress>
{
    std::size_t operator()(const xmsg::RegAddress& k) const
    {
        using std::hash;

        return hash<std::string>()(k.host()) ^ (hash<int>()(k.port()) << 1);
    }
};

} // end namespace std

#endif // XMSG_CORE_ADDRESS_H_
