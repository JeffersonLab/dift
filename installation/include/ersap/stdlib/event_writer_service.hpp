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

#ifndef ERSAP_STD_EVENT_WRITER_HPP
#define ERSAP_STD_EVENT_WRITER_HPP

#include <ersap/engine.hpp>
#include <ersap/third_party/json11.hpp>

#include <memory>
#include <set>
#include <stdexcept>

namespace ersap {
namespace stdlib {

/**
 * An abstract writer service that writes all received events into the
 * configured output file.
 */
class EventWriterService : public Engine
{
public:
    EventWriterService();
    virtual ~EventWriterService();

public:
    EngineData configure(EngineData&) override;

    EngineData execute(EngineData& input) override;

    EngineData execute_group(const std::vector<EngineData>&) override;

protected:
    /**
     * A problem in the event writer implementation.
     */
    class EventWriterError : public std::runtime_error
    {
    public:
        EventWriterError(const char* msg)
          : std::runtime_error{msg}
        {}

        EventWriterError(const std::string& msg)
          : std::runtime_error{msg}
        {}
    };

    enum class Endian
    {
        Little,
        Big,
    };

    static Endian parse_byte_order(const json11::Json& opts);

private:
    /**
     * Creates a new writer and opens the given output file.
     *
     * @param file the path to the output file
     * @param opts extra options for the writer
     * @throws EventWriterError if the file could not be opened
     */
    virtual void open_file(const std::string& file,
                           const json11::Json& opts) = 0;

    /**
     * Closes the output file.
     */
    virtual void close_file() = 0;

    /**
     * Returns true if an output file is open.
     */
    virtual bool has_file() = 0;

    /**
     * Writes an event to the output file.
     * The event should be a C++ object with the same type as the one defined
     * by the ERSAP engine data-type returned by {@link #get_data_type()}.
     *
     * @param event the event to be written
     * @throws EventWriterError if the file could not be read
     */
    virtual void write_event(const any& event) = 0;

    /**
     * Gets the ERSAP engine data-type for the type of the events.
     * The data-type will be used to deserialize the events when the engine data
     * is received from services across the network.
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

#endif // end of include guard: ERSAP_STD_EVENT_WRITER_HPP
