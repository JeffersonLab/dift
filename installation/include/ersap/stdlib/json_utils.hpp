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

#ifndef ERSAP_STD_JSON_UTILS_HPP
#define ERSAP_STD_JSON_UTILS_HPP

#include <ersap/third_party/json11.hpp>

#include <iostream>
#include <sstream>

namespace ersap {

class EngineData;

namespace stdlib {

/**
  * A problem parsing JSON data
  */
class JsonError : public std::runtime_error
{
public:
    JsonError(const char* msg)
        : std::runtime_error{msg}
    {}

    JsonError(const std::string& msg)
        : std::runtime_error{msg}
    {}
};


json11::Json parse_json(const EngineData& input);

json11::Json parse_json(const std::string& str);

bool has_key(const json11::Json& obj, const std::string& key);

bool get_bool(const json11::Json& obj, const std::string& key);

int get_int(const json11::Json& obj, const std::string& key);

int get_double(const json11::Json& obj, const std::string& key);

const std::string& get_string(const json11::Json& obj, const std::string& key);

const json11::Json& get_array(const json11::Json& obj, const std::string& key);

const json11::Json& get_object(const json11::Json& obj, const std::string& key);

void check_json(const json11::Json& obj, const json11::Json::shape& shape);

} // end namespace stdlib
} // end namespace ersap

#endif // end of include guard: ERSAP_STD_JSON_UTILS_HPP
