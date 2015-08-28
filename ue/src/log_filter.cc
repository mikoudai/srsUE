/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2015 The srsUE Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution.
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include <boost/date_time/posix_time/posix_time.hpp>
#include "log_filter.h"

using namespace srslte;

namespace srsue{

log_filter::log_filter(std::string layer, ue_logger *logger_)
  :log(layer)
  ,logger(logger_)
{}

void log_filter::all_log(srslte::LOG_LEVEL_ENUM level,
                         uint32_t               tti,
                         char                  *msg)
{
  if(logger) {
    std::stringstream ss;

    ss << now_time() << " ";
    ss << "[" <<get_service_name() << "] ";
    ss << log_level_text[level] << " ";
    ss << "[" << tti << "] ";
    ss << msg;

    str_ptr s_ptr(new std::string(ss.str()));
    logger->log(s_ptr);
  }
}

void log_filter::all_log(srslte::LOG_LEVEL_ENUM level,
                         uint32_t               tti,
                         char                  *msg,
                         uint8_t               *hex,
                         int                    size)
{
  if(logger) {
    std::stringstream ss;

    ss << now_time() << " ";
    ss << get_service_name() << " ";
    ss << log_level_text[level] << " ";
    ss << "[" << tti << "] ";
    ss << msg << std::endl;
    ss << hex_string(hex, size);

    str_ptr s_ptr(new std::string(ss.str()));
    logger->log(s_ptr);
  }
}

void log_filter::error(std::string message, ...) {
  if (level >= LOG_LEVEL_ERROR) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_ERROR, tti, args_msg);
    va_end(args);
    free(args_msg);
  }
}
void log_filter::warning(std::string message, ...) {
  if (level >= LOG_LEVEL_WARNING) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_WARNING, tti, args_msg);
    va_end(args);
    free(args_msg);
  }
}
void log_filter::info(std::string message, ...) {
  if (level >= LOG_LEVEL_INFO) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_INFO, tti, args_msg);
    va_end(args);
    free(args_msg);
  }
}
void log_filter::debug(std::string message, ...) {
  if (level >= LOG_LEVEL_DEBUG) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_DEBUG, tti, args_msg);
    va_end(args);
    free(args_msg);
  }
}

void log_filter::error_hex(uint8_t *hex, int size, std::string message, ...) {
  if (level >= LOG_LEVEL_ERROR) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_ERROR, tti, args_msg, hex, size);
    va_end(args);
    free(args_msg);
  }
}
void log_filter::warning_hex(uint8_t *hex, int size, std::string message, ...) {
  if (level >= LOG_LEVEL_WARNING) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_WARNING, tti, args_msg, hex, size);
    va_end(args);
    free(args_msg);
  }
}
void log_filter::info_hex(uint8_t *hex, int size, std::string message, ...) {
  if (level >= LOG_LEVEL_INFO) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_INFO, tti, args_msg, hex, size);
    va_end(args);
    free(args_msg);
  }
}
void log_filter::debug_hex(uint8_t *hex, int size, std::string message, ...) {
  if (level >= LOG_LEVEL_DEBUG) {
    char     *args_msg;
    va_list   args;
    va_start(args, message);
    if(vasprintf(&args_msg, message.c_str(), args) > 0);
      all_log(LOG_LEVEL_DEBUG, tti, args_msg, hex, size);
    va_end(args);
    free(args_msg);
  }
}

std::string log_filter::now_time()
{
  using namespace boost::posix_time;
  return std::string(to_simple_string(microsec_clock::local_time()), 12, 12);
}

std::string log_filter::hex_string(uint8_t *hex, int size)
{
  std::stringstream ss;
  int c = 0;

  ss << std::hex << std::setfill('0');
  size = (size > hex_limit) ? hex_limit : size;
  while(c < size) {
    ss << "       " << std::setw(4) << static_cast<unsigned>(c) << ": ";
    int tmp = (size-c < 16) ? size-c : 16;
    for(int i=0;i<tmp;i++) {
      ss << std::setw(2) << static_cast<unsigned>(hex[c++]) << " ";
    }
    ss << "\n";
  }
  return ss.str();
}

} // namespace srsue
