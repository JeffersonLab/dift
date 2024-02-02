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

#ifndef XMSG_CORE_XMSG_H_
#define XMSG_CORE_XMSG_H_

#include <xmsg/address.h>
#include <xmsg/connection.h>
#include <xmsg/message.h>
#include <xmsg/proto/registration.h>
#include <xmsg/subscription.h>
#include <xmsg/topic.h>

#include <functional>
#include <memory>
#include <set>
#include <string>

/**
 * Core %xMsg classes and functions.
 */
namespace xmsg {

class ConnectionSetup;


/**
 * The main %xMsg pub/sub actor.
 *
 * Actors send messages to each other using pub/sub communications
 * through a cloud of %xMsg proxies.
 * Registrar services provide registration and discoverability of actors.
 *
 * An actor has a _name_ for identification, a _default proxy_ intended for
 * long-term publication/subscription of messages, and a _default registrar_
 * where it can register and discover other long-term actors.
 * Unless otherwise specified, the local node and the standard ports will be
 * used for both default proxy and registrar.
 *
 * Publishers set specific _topics_ for their messages, and subscribers define
 * topics of interest to filter which messages they want to receive.
 * A _domain-specific callback_ defined by the subscriber will be executed every
 * time a message is received. This callback must be thread-safe,
 * and it can also be used to send responses or new messages.
 *
 * In order to publish or subscribe to messages, a _connection_ to a proxy must
 * be obtained. The actor owns and keeps a _pool of available connections_,
 * creating new ones as needed. When no address is specified, the _default
 * proxy_ will be used. The connections can be returned to the pool of available
 * connections, to avoid creating too many new connections. All connections will
 * be closed when the actor is destroyed.
 *
 * Multi-threaded publication of messages is fully supported, but every thread
 * must use its own connection. Subscriptions of messages always run in their
 * own background thread. It is recommended to always obtain and release the
 * necessary connections inside the thread that uses them. The _connect_ methods
 * will ensure that each thread gets a different connection.
 *
 * Publishers must be sending messages through the same _proxy_ than the
 * subscribers for the messages to be received. Normally, this proxy will be
 * the _default proxy_ of a long-term subscriber with many dynamic publishers, or
 * the _default proxy_ of a long-term publisher with many dynamic subscribers.
 * To have many publishers sending messages to many subscribers, they all must
 * _agree_ in the proxy. It is possible to use several proxies, but multiple
 * publications and subscriptions will be needed, and it may get complicated.
 * Applications using %xMsg have great flexibility to organize their
 * communications, but it is better to deploy simple topologies.
 *
 * Actors can register as publishers and/or subscribers with _registrar services_,
 * so other actors can discover them if they share the topic of interest.
 * Using the registration and discovery methods is always thread-safe.
 * The registrar service must be common to the actors, running in a known node.
 * If no address is specified, the _default registrar_ will be used.
 * Note that the registration will always set the _default proxy_ as the proxy
 * through which the actor is publishing/subscribed to messages.
 * If registration for different proxies is needed, multiple actors should be
 * used, each one with an appropriate default proxy.
 *
 * The proxy and the registrar are provided as stand-alone executables,
 * but only the Java implementation can be used to run a registrar.
 *
 * \see Message
 * \see Topic
 */
class xMsg
{
public:
    /**
     * Creates an actor with default settings.
     * The local node and the standard ports will be used for both
     * default proxy and registrar.
     *
     * \param name the name of this actor
     */
    explicit xMsg(const std::string& name);

    /**
     * Creates an actor specifying the default registrar to be used.
     * The local node and the standard ports will be used for the default proxy.
     *
     * \param name the name of this actor
     * \param default_registrar the address to the default registrar
     */
    explicit xMsg(const std::string& name,
                  const RegAddress& default_registrar);

    /**
     * Creates an actor specifying the default proxy and registrar to be used.
     *
     * \param name the name of this actor
     * \param default_proxy the address to the default proxy
     * \param default_registrar the address to the default registrar
     */
    explicit xMsg(const std::string& name,
                  const ProxyAddress& default_proxy,
                  const RegAddress& default_registrar);

    xMsg(const xMsg& rhs) = delete;
    xMsg& operator=(const xMsg& rhs) = delete;

    xMsg(xMsg&& rhs) noexcept;
    xMsg& operator=(xMsg&& rhs) noexcept;

    virtual ~xMsg();

public:
    /**
     * Obtains a connection to the default proxy.
     * If there is no available connection, a new one will be created.
     */
    ProxyConnection connect();

    /**
     * Obtains a connection to the specified proxy.
     * If there is no available connection, a new one will be created.
     *
     * \param addr the address of the proxy
     */
    ProxyConnection connect(const ProxyAddress& addr);

    /**
     * Changes the setup of all created connections.
     * The new setup will be used for every new connection.
     *
     * \param setup the new default connection setup
     */
    void set_connection_setup(std::unique_ptr<ConnectionSetup> setup);

public:
    /**
     * Publishes a message through the specified proxy connection.
     *
     * \param connection the connection to the proxy
     * \param msg the message to be published
     */
    void publish(ProxyConnection& connection, Message& msg);

    /**
     * Publishes a message through the specified proxy connection and blocks
     * waiting for a response.
     *
     * The subscriber must publish the response to the topic given by the
     * `replyto` metadata field, through the same proxy.
     *
     * This method will throw if a response is not received before the timeout
     * expires.
     *
     * \param connection the connection to the proxy
     * \param msg the message to be published
     * \param timeout the length of time to wait a response, in milliseconds
     * \return the response message
     */
    Message sync_publish(ProxyConnection& connection, Message& msg, int timeout);

    /**
     * Subscribes to a topic of interest through the specified proxy
     * connection.
     * A background thread will be started to receive the messages.
     *
     * \param topic the topic to select messages
     * \param connection the connection to the proxy
     * \param callback the user action to run when a message is received
     */
    std::unique_ptr<Subscription>
    subscribe(const Topic& topic,
              ProxyConnection&& connection,
              std::function<void(Message&)> callback);

    /**
     * Stops the given subscription.
     *
     * \param handler an active subscription
     */
    void unsubscribe(std::unique_ptr<Subscription> handler);

public:
    /**
     * Registers this actor as a publisher of the specified topic,
     * on the default registrar service.
     *
     * The actor will be registered as publishing through the default proxy.
     *
     * \param topic the topic to which messages will be published
     * \param description general description of the published messages
     */
    void register_as_publisher(const Topic& topic,
                               const std::string& description);

    /**
     * Registers this actor as a publisher of the specified topic,
     * on the given registrar service.
     *
     * The actor will be registered as publishing through the default proxy.
     *
     * \param addr the address to the registrar service
     * \param topic the topic to which messages will be published
     * \param description general description of the published messages
     */
    void register_as_publisher(const RegAddress& addr,
                               const Topic& topic,
                               const std::string& description);

    /**
     * Registers this actor as a subscriber of the specified topic,
     * on the default registrar service.
     *
     * The actor will be registered as subscribed through the default proxy.
     *
     * \param topic the topic of the subscription
     * \param description general description of the subscription
     */
    void register_as_subscriber(const Topic& topic,
                                const std::string& description);

    /**
     * Registers this actor as a subscriber of the specified topic,
     * on the given registrar service.
     *
     * The actor will be registered as subscribed through the default proxy.
     *
     * \param addr the address to the registrar service
     * \param topic the topic of the subscription
     * \param description general description of the subscription
     */
    void register_as_subscriber(const RegAddress& addr,
                                const Topic& topic,
                                const std::string& description);

    /**
     * Removes this actor as a publisher of the specified topic,
     * from the default registrar service.
     *
     * \param topic the topic to which messages were published
     */
    void deregister_as_publisher(const Topic& topic);

    /**
     * Removes this actor as a publisher of the specified topic,
     * from the given registrar service.
     *
     * \param addr the address to the registrar service
     * \param topic the topic to which messages were published
     */
    void deregister_as_publisher(const RegAddress& addr, const Topic& topic);

    /**
     * Removes this actor as a subscriber of the specified topic,
     * from the default registrar service.
     *
     * \param topic the topic of the subscription
     */
    void deregister_as_subscriber(const Topic& topic);

    /**
     * Removes this actor as a subscriber of the specified topic,
     * from the given registrar service.
     *
     * \param addr the address to the registrar service
     * \param topic the topic of the subscription
     */
    void deregister_as_subscriber(const RegAddress& addr, const Topic& topic);

    /**
     * Finds all publishers of the specified topic
     * that are registered on the default registrar service.
     *
     * \param topic the topic of interest
     * \return a set with the registration data of the matching publishers
     * \see Topic for the rules of matching topics
     */
    RegDataSet find_publishers(const Topic& topic);

    /**
     * Finds all publishers of the specified topic
     * that are registered on the given registrar service.
     *
     * \param addr the address to the registrar service
     * \param topic the topic of interest
     * \return a set with the registration data of the matching publishers
     * \see Topic for the rules of matching topics
     */
    RegDataSet find_publishers(const RegAddress& addr, const Topic& topic);

    /**
     * Finds all subscribers to the specified topic
     * that are registered on the default registrar service.
     *
     * \param topic the topic of interest
     * \return a set with the registration data of the matching subscribers
     * \see Topic for the rules of matching topics
     */
    RegDataSet find_subscribers(const Topic& topic);

    /**
     * Finds all subscribers to the specified topic
     * that are registered on the given registrar service.
     *
     * \param addr the address to the registrar service
     * \param topic the topic of interest
     * \return a set with the registration data of the matching subscribers
     * \see Topic for the rules of matching topics
     */
    RegDataSet find_subscribers(const RegAddress& addr, const Topic& topic);

public:
    /**
     * Returns the name of this actor
     */
    const std::string& name() const;

    /**
     * Returns the address of the default registrar service used by this actor
     */
    const RegAddress& default_registrar() const;

    /**
     * Returns the address of the default proxy used by this actor
     */
    const ProxyAddress& default_proxy() const;

private:
    struct Impl;
    std::unique_ptr<Impl> xmsg_;
};

} // end namespace xmsg

#endif // XMSG_CORE_XMSG_H_
