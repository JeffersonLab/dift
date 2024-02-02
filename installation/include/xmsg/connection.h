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

#ifndef XMSG_CORE_CONNECTION_H_
#define XMSG_CORE_CONNECTION_H_

#include <xmsg/address.h>
#include <xmsg/message.h>

#include <memory>

namespace xmsg {

namespace detail {

class ProxyDriver;
class RegDriver;

struct ProxyDriverDeleter
{
    void operator()(ProxyDriver* p);
};

struct RegDriverDeleter
{
    void operator()(RegDriver* p);
};

using ProxyDriverPtr = std::unique_ptr<ProxyDriver, ProxyDriverDeleter>;
using RegDriverPtr = std::unique_ptr<RegDriver, RegDriverDeleter>;


} // end namespace detail

class ConnectionPool;


template<typename A, typename U>
class ScopedConnection
{
public:
    using pointer = typename U::pointer;
    using element_type = typename U::element_type;
    using deleter = std::function<void(U&&)>;

private:
    ScopedConnection(const A& addr, U&& con, deleter&& del)
      : addr_{addr}
      , con_{std::move(con)}
      , del_{std::move(del)}
    {
        // nop
    }

public:
    ScopedConnection(const ScopedConnection&) = delete;
    ScopedConnection& operator=(const ScopedConnection&) = delete;

    ScopedConnection(ScopedConnection&&) noexcept(std::is_nothrow_move_constructible<deleter>::value) = default;
    ScopedConnection& operator=(ScopedConnection&&) noexcept(std::is_nothrow_move_constructible<deleter>::value) = default;

    ~ScopedConnection()
    {
        if (con_) {
            del_(std::move(con_));
        }
    };

public:
    const A& address() const
    {
        return addr_;
    }

    pointer get() const
    {
        return con_.get();
    }

    void close()
    {
        con_.reset();
    }

private:
    typename std::add_lvalue_reference<element_type>::type
    operator*() const
    {
        return *con_;
    }

    pointer operator->() const
    {
        return get();
    }

    U release()
    {
        auto con = std::move(con_);
        return con;
    }

private:
    friend ConnectionPool;
    friend xMsg;

    A addr_;
    U con_;
    deleter del_;
};

using ProxyConnection = ScopedConnection<ProxyAddress, detail::ProxyDriverPtr>;
using RegConnection = ScopedConnection<RegAddress, detail::RegDriverPtr>;

} // end namespace xmsg

#endif // XMSG_CORE_CONNECTION_H_
