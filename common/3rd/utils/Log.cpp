//
// Created by 听见 on 2020/10/27.
//

#include <utility>

#include "Log.h"
uint16_t ANDROID_LOG_ALL = ANDROID_LOG_DEFAULT;

bool logLevel = ANDROID_LOG_ALL;

std::chrono::steady_clock::time_point currentTime() {
  return std::chrono::steady_clock::now();
}

Logger::Logger(std::string tag) : tag(std::move(tag)) {}

void Logger::log(int32_t level, const char *tag, const char *format, va_list vaList) {
  if (level > logLevel) {
    __android_log_vprint(level, tag, format, vaList);
  }
}

void Logger::log(int32_t level, const char *tag, const char *format, ...) {
  va_list arglist;
  va_start(arglist, format);
  log(level, tag, format, arglist);
  va_end(arglist);
}

void Logger::log(int32_t level, const char *format, ...) {
  va_list arglist;
  va_start(arglist, format);
  log(level, tag.c_str(), format, arglist);
  va_end(arglist);
}

void Logger::loge(const char *format, ...) {
  va_list arglist;
  va_start(arglist, format);
  log(ANDROID_LOG_ERROR, format, arglist);
  va_end(arglist);
}
void Logger::logw(const char *format, ...) {
  va_list arglist;
  va_start(arglist, format);
  log(ANDROID_LOG_WARN, format, arglist);
  va_end(arglist);
}
void Logger::logd(const char *format, ...) {
  va_list arglist;
  va_start(arglist, format);
  log(ANDROID_LOG_DEBUG, format, arglist);
  va_end(arglist);
}
void Logger::logi(const char *format, ...) {
  va_list arglist;
  va_start(arglist, format);
  log(ANDROID_LOG_INFO, format, arglist);
  va_end(arglist);
}

