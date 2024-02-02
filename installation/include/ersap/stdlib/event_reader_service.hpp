/*
 * Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 * to use, copy, modify, and distribute  this software and its documentation for
 * educational, research, and not-for-profit purposes, without fee and without a
 * signed licensing agreement.
 *
 * IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 * OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE ERSAP SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY,
 * PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO PROVIDE
 * MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * This software was developed under the United States Government license.
 * For more information contact author at gurjyan@jlab.org
 * Department of Experimental Nuclear Physics, Jefferson Lab.
 */

#ifndef ERSAP_STD_EVENT_READER_HPP
#define ERSAP_STD_EVENT_READER_HPP

#include <ersap/engine.hpp>
#include <ersap/third_party/json11.hpp>

#include <memory>
#include <set>
#include <stdexcept>

namespace ersap {
namespace stdlib {


/**
 * An abstract reader service that reads events from the configured input file.
 */
class EventReaderService : public Engine
{
public:
    EventReaderService();
    virtual ~EventReaderService();

public:
    EngineData configure(EngineData&) override;

    EngineData execute(EngineData& input) override;

    EngineData execute_group(const std::vector<EngineData>&) override;

protected:
    enum class Endian
    {
        Little,
        Big,
    };

    /**
     * A problem in the event reader implementation.
     */
    class EventReaderError : public std::runtime_error
    {
    public:
        EventReaderError(const char* msg)
          : std::runtime_error{msg}
        {}

        EventReaderError(const std::string& msg)
          : std::runtime_error{msg}
        {}
    };

private:
    /**
     * Opens the given input file.
     *
     * @param file the path to the input file
     * @param opts extra options for the reader
     * @throws EventReaderError if the file could not be open
     */
    virtual void open_file(const std::string& file,
                           const json11::Json& opts) = 0;

    /**
     * Closes the input file.
     */
    virtual void close_file() = 0;

    /**
     * Returns true if an input file is open.
     */
    virtual bool has_file() = 0;

    /**
     * Reads an event from the input file.
     * The event should be a C++ object with the same type as the one defined
     * by the ERSAP engine data-type returned by {@link #get_data_type()}.
     *
     * @param event_number the index of the event in the file (starts from zero)
     * @return the read event
     * @throws EventReaderError if the file could not be read
     */
    virtual ersap::any read_event(int event_number) = 0;

    /**
     * Gets the total number of events that can be read from the input file.
     *
     * @return the total number of events in the file
     * @throws EventReaderError if the file could not be read
     */
    virtual size_t read_event_count() = 0;

    /**
     * Gets the byte order of the events stored in the input file.
     *
     * @return the byte order of the events in the file
     * @throws EventReaderError if the file could not be read
     */
    virtual Endian read_byte_order() = 0;

    /**
     * Gets the ERSAP engine data-type for the type of the events.
     * The data-type will be used to serialize the events when the engine data
     * result needs to be sent to services over the network.
     *
     * @return the data-type of the events
     */
    virtual const EngineDataType& get_data_type() const = 0;

public:
    std::vector<EngineDataType> input_data_types() const override;

    std::vector<EngineDataType> output_data_types() const override;

    std::set<std::string> states() const override;

public:
    void reset() override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // end namespace stdlib
} // end namespace ersap


#endif // end of include guard: ERSAP_STD_EVENT_READER_HPP
