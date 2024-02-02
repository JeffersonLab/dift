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

#ifndef XMSG_CORE_ADVANCED_H_
#define XMSG_CORE_ADVANCED_H_

#include <cstddef>
#include <type_traits>

namespace xmsg {

/**
 * Sets options on a new ZMQ socket.
 *
 * \see <a href="http://api.zeromq.org/4-1:zmq-setsockopt">zmq_setsockopt</a>
 */
class SocketSetup
{
public:
    explicit SocketSetup(void* socket)
      : socket_{socket}
    { }

    SocketSetup(const SocketSetup& rhs) = delete;
    SocketSetup& operator=(const SocketSetup& rhs) = delete;

    SocketSetup(SocketSetup&&) noexcept = default;
    SocketSetup& operator=(SocketSetup&&) noexcept = default;

    ~SocketSetup() = default;

public:
    /// Sets the value of a ØMQ socket option
    void set_option(int opt, const void* val, size_t val_len);

    /// Sets the value of a ØMQ socket option
    template <typename Integer,
              typename = std::enable_if_t<std::is_integral<Integer>::value>>
    void set_option(int opt, const Integer& val)
    {
        set_option(opt, &val, sizeof(Integer));
    }

    /// Gets the value of a ØMQ socket option
    void get_option(int opt, void* val, size_t* val_len) const;

    /// Gets the value of a ØMQ socket option
    template <typename Integer,
              typename = std::enable_if_t<std::is_integral<Integer>::value>>
    Integer get_option(int opt) const
    {
        Integer val;
        size_t val_len = sizeof(Integer);
        get_option(opt, &val, &val_len);
        return val;
    }

    /// Gets the type of the 0MQ socket
    int type() const;

private:
    void* socket_;
};


/**
 * Advanced setup of a connection to an %xMsg proxy.
 *
 * This class can be used to customize the internal sockets of a new Connection
 * created by the xMsg actor.
 */
class ConnectionSetup
{
public:
    ConnectionSetup() = default;

    ConnectionSetup(const ConnectionSetup&) = delete;
    ConnectionSetup& operator=(const ConnectionSetup&) = delete;

    virtual ~ConnectionSetup() = default;

    /**
     * Configures the socket before it is connected.
     * This method will be called for both pub/sub sockets.
     * It should be used to set options on the socket.
     *
     * Leave empty if no configuration is required.
     */
    virtual void pre_connection(SocketSetup& /*socket*/) {};

    /**
     * Runs after the two sockets have been connected.
     * This method can be used to run some action after connecting the sockets.
     * For example, sleep a while to give time to the sockets to be actually
     * connected internally.
     *
     * Leave empty if no action is required.
     */
    virtual void post_connection() {};
};

} // end namespace xmsg

#endif // XMSG_CORE_ADVANCED_H_
