#pragma once

// these log messages are output uing the '<<' operator, so to make
// your class compatible you must overload it with the following:
//
// friend std::ostream& operator<<(std::ostream& os, const MyClass& my_class);

#ifdef _DEBUG

  #define LOGGING_ENABLED
  #include "DebugLog_HelperMacros.h"

  // clears the log file
  #define ERASE_LOG                                                                               WRAP_(Debug::LOGGING_FILE.open(Debug::LOGGING_FILENAME, std::ofstream::out | std::ofstream::trunc); Debug::LOGGING_FILE.close();)
  
  // when logging, if you want to chain together multiple inputs to log,
  // use the '<<' operator, such as:
  //
  // LOG( "Hello World" << 5 << 4.15f << std::string("Hello") << my_class_with_overloaded_outstream )

  // basic unconditional log macro
  #define LOG(MESSAGE)                                                                            LOG_(MESSAGE, ' ')
  
  // unconditional log macro that lets you place a special
  // character in the 0th column for easier identification
  #define LOG_MARKED(MESSAGE, MARK)                                                               LOG_(MESSAGE, MARK)

  // basic conditional log macro
  #define LOG_IF(MESSAGE, CONDITION)                                                              if(CONDITION) LOG(MESSAGE);
  
  // conditional log macro that lets you place a special
  // character in the 0th column for easier identification
  #define LOG_MARKED_IF(MESSAGE, CONDITION, MARK)                                                 if(CONDITION) LOG_MARKED(MESSAGE, MARK);

#else

  // empty macro equivalents for Release builds

  #define ERASE_LOG
  #define LOG(MESSAGE)
  #define LOG_MARKED(MESSAGE, MARK)
  #define LOG_IF(MESSAGE, CONDITION)
  #define LOG_MARKED_IF(MESSAGE, CONDITION, MARK)

#endif