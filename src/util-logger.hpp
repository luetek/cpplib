// Copyright 2023 code.luetek.com
//! \file util_logger.h
//!
//! \author Dan Yerushalmi (Picked from original author. Thanks )
//!
//! \date 20/10/2016
//!
//! \brief Based on http://stackoverflow.com/questions/24750218/boost-log-to-print-source-code-file-name-and-line-number
//!        I have modified this code to work as thread safe with underlaying boost trvial logger.
//!        Can change filter and output at runtime.
//!
//! \copyright (MIT) please give me credited by keeping my name on header, thanks.
#pragma once

#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

// New macro that includes severity, filename and line number
#define LOG_ADD_FILE boost::log::attribute_cast<boost::log::attributes::mutable_constant<std::string>>(::boost::log::trivial::logger::get().get_attributes()["File"]).set(logger::path_to_filename(__FILE__)) // NOLINT
#define LOG_ADD_LINE boost::log::attribute_cast<boost::log::attributes::mutable_constant<int>>(::boost::log::trivial::logger::get().get_attributes()["Line"]).set(__LINE__)                                   // NOLINT



#define LOG__(sev) \
    LOG_ADD_FILE;  \
    LOG_ADD_LINE;  \
    BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(), (::boost::log::keywords::severity = ::boost::log::trivial::sev)) // NOLINT

#define LOG_MSG(sev, msg) \
    LOG__(sev) << msg
// NOLINTBEGIN
// Compile with -DMAX_LOG_LEVEL=info to statically deactivate all log messages below info.
#define LOG(sev) \
    if constexpr(boost::log::trivial::sev >= boost::log::trivial::MAX_LOG_LEVEL) LOG__(sev)
// NOLINTEND
namespace logger {

    // Set attribute and return the new value
    template <typename ValueType>
    ValueType set_get_attrib(const char *name, ValueType value) {
        auto attr = logging::attribute_cast<attrs::mutable_constant<ValueType>>
            (boost::log::trivial::logger::get().get_attributes()[name]);
        attr.set(value);
        return attr.get();
    }

    // Convert file path to only the filename
    inline std::string path_to_filename(std::string path) {
        return path.substr(path.find_last_of("/\\") + 1);
    }

    inline void init(boost::log::trivial::severity_level svl) {
        // New attributes that hold filename and line number for trvial logger.
        boost::log::trivial::logger::get().add_attribute("File", attrs::mutable_constant<std::string>(""));
        boost::log::trivial::logger::get().add_attribute("Line", attrs::mutable_constant<int>(0));

        logging::add_console_log(std::cout, keywords::format =
                (expr::stream
                    << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d_%H:%M:%S.%f")
                    << ": " << boost::log::trivial::severity << " "
                    << expr::format_named_scope("Scope", keywords::format = " %n ")
                    << '[' << expr::attr<std::string>("File")
                    << ':' << expr::attr<int>("Line") << "] "
                    << expr::smessage));

        boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
        // register to common attributes:
        logging::add_common_attributes();

        // set severity:
        logging::core::get()->set_filter(
            logging::trivial::severity >= svl);
    }

    // use during runtime to change severity log output.
    inline void set_log_filter(boost::log::trivial::severity_level svl) {
        logging::core::get()->set_filter(
            logging::trivial::severity >= svl);
    }
}  // namespace logger
