#ifndef FRAMEWORK_LOGGING_H_
#define FRAMEWORK_LOGGING_H_

#include <string>
#include <sstream>
#include <iostream>

namespace framework {

struct DummyLogger {
  DummyLogger() {}
  template<typename T>
  DummyLogger& operator<<(const T& info) {}
};

struct Logger : public DummyLogger {
  struct Level {
    enum class Severity : int {
      NONE,DEBUG,INFO,WARN,ERROR,FATAL,
    };
  };

  enum class Targets : int {
    CONSOLE,FILE,STREAM,
  };

  static Targets target;
  template <Targets tg = Targets::CONSOLE>
  struct Output {
  };

  static bool WillCreateLogger(Level::Severity level) {
    return level > Level::Severity::NONE;
  }

  Logger(Level::Severity level, const char* file, int line);

  template<typename T>
  Logger& operator << (const T& info) {
    stream_ << info;
    if (*str().rbegin() != '\n')
      stream_ << '\n';
    std::cout << stream_.str() << std::flush;
    return *this;
  }

  std::string str() {
    return stream_.str();
  }
private:
  void Init();
  std::string LevelToString(Level::Severity level);

private:
  Level::Severity level_ = Level::Severity::DEBUG;
  std::string file_;
  int line_;
  std::ostringstream stream_;
};

} // namespace framework

#define LOG_ENABLE(level) \
  ::framework::Logger::WillCreateLogger(level)

#define LOGGER(level) \
  ::framework::Logger(level, __FILE__, __LINE__)

#define LOGGING(level, condition) \
  !condition ? ::framework::DummyLogger() : LOGGER(level)

#define LOG(level) \
  LOGGING(level, LOG_ENABLE(level))

using LEVEL = ::framework::Logger::Level::Severity;
const LEVEL DEBUG = ::framework::Logger::Level::Severity::DEBUG;
const LEVEL INFO  = ::framework::Logger::Level::Severity::INFO;
const LEVEL WARN  = ::framework::Logger::Level::Severity::WARN;
const LEVEL ERROR = ::framework::Logger::Level::Severity::ERROR;
const LEVEL FATAL = ::framework::Logger::Level::Severity::FATAL;

#endif
