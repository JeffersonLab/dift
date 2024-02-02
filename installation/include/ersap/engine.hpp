

#ifndef ERSAP_ENGINE_HPP
#define ERSAP_ENGINE_HPP

#include <ersap/engine_data.hpp>
#include <ersap/engine_data_type.hpp>

#include <memory>
#include <set>
#include <string>
#include <vector>

/**
 *
 * Core ERSAP classes and functions.
 */
namespace ersap {

class Engine
{
public:
    virtual EngineData configure(EngineData&) = 0;

    virtual EngineData execute(EngineData&) = 0;

    virtual EngineData execute_group(const std::vector<EngineData>&) = 0;

public:
    virtual std::vector<EngineDataType> input_data_types() const = 0;

    virtual std::vector<EngineDataType> output_data_types() const = 0;

    virtual std::set<std::string> states() const { return std::set<std::string>{};  }

public:
    virtual std::string name() const = 0;

    virtual std::string author() const = 0;

    virtual std::string description() const = 0;

    virtual std::string version() const = 0;

public:
    virtual void reset() { };

    virtual ~Engine() = default;
};

} // end namespace ersap

#endif // end of include guard: ERSAP_ENGINE_HPP
