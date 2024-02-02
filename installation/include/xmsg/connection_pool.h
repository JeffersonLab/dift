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

#ifndef XMSG_CORE_CONNECTION_POOL_H_
#define XMSG_CORE_CONNECTION_POOL_H_

#include <xmsg/connection.h>
#include <xmsg/connection_setup.h>
#include <xmsg/context.h>

#include <memory>

namespace xmsg {

class ConnectionPool
{
public:
    ConnectionPool();
    explicit ConnectionPool(std::shared_ptr<Context> ctx);

    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;

    ConnectionPool(ConnectionPool&& rhs) noexcept;
    ConnectionPool& operator=(ConnectionPool&& rhs) noexcept;

    virtual ~ConnectionPool();

public:
    ProxyConnection get_connection(const ProxyAddress& addr);

    RegConnection get_connection(const RegAddress& addr);

public:
    void set_default_setup(std::unique_ptr<ConnectionSetup> setup);

private:
    virtual detail::ProxyDriverPtr create_connection(const ProxyAddress& addr);
    virtual detail::RegDriverPtr create_connection(const RegAddress& addr);

private:
    template<typename A, typename U>
    class ConnectionCache;

    using ProxyDriverCache = ConnectionCache<ProxyAddress, detail::ProxyDriverPtr>;
    using RegDriverCache = ConnectionCache<RegAddress, detail::RegDriverPtr>;

    std::shared_ptr<Context> ctx_;
    std::shared_ptr<ConnectionSetup> setup_;

    std::unique_ptr<ProxyDriverCache> proxy_cache_;
    std::unique_ptr<RegDriverCache> reg_cache_;
};

} // end namespace xmsg

#endif // XMSG_CORE_CONNECTION_POOL_H_
