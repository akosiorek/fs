/*
 * Utils.h
 *
 *  Created on: Aug 8, 2014
 *      Author: Adam Kosiorek
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <type_traits>
#include <stdexcept>
#include <glog/logging.h>

#define THROW_ERROR(T, msg) \
	static_assert(std::is_base_of<std::exception, T>::value, "T should be an exception type"); \
	LOG(ERROR) << msg; \
	throw T(msg)

#define THROW_RUNTIME(msg) THROW_ERROR(std::runtime_error, msg);
#define THROW_LOGIC(msg) THROW_ERROR(std::logic_error, msg);
#define THROW_ARG(msg) THROW_ERROR(std::invalid_argument, msg);

struct Utils {

	template <class T>
	static void error(const std::string& msg) {
		static_assert(std::is_base_of<std::exception, T>::value, "T should be an exception type");
		LOG(ERROR) << msg;
		throw T(msg);
	}

	static void throw_runtime(const std::string& msg) {
		error<std::runtime_error>(msg);
	}
};

#endif /* UTILS_H_ */
