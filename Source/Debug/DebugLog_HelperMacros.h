#pragma once
// THIS FILE SHOULD NOT BE INCLUDED ANYWHERE OTHER THAN DebugLog.h

#ifdef LOGGING_ENABLED

  // ignore these macros, they are used by the ones in DebugLog.h to
  // streamline the creation of more complex macros
  #define WRAP_(STATEMENTS)                                                     {STATEMENTS}
  #define CUR_FILENAME_                                                         Debug::FILENAME(__FILE__)
  #define OPEN_FILE_                                                            Debug::LOGGING_FILE.open(Debug::LOGGING_FILENAME, std::ofstream::app);
  #define ADD_TO_FILE_(MESSAGE)                                                 Debug::LOGGING_FILE << MESSAGE;
  #define ADD_NEWLINE_FILE_                                                     Debug::LOGGING_FILE << '\n';
  #define CLOSE_FILE_                                                           ADD_NEWLINE_FILE_ Debug::LOGGING_FILE.close();
  #define LOG_(MESSAGE, INFO_CHAR)                                              WRAP_(OPEN_FILE_ ADD_TO_FILE_(INFO_CHAR) ADD_TO_FILE_(' ') ADD_TO_FILE_(CUR_FILENAME_) ADD_TO_FILE_(" (") ADD_TO_FILE_(__LINE__) ADD_TO_FILE_("): ") ADD_TO_FILE_('\t') ADD_TO_FILE_(MESSAGE) CLOSE_FILE_)
  
  
  #include <fstream> // ofstream
  
  namespace Debug
  {
    // the location and name of the log file (relative to *.exe)
    static const char* LOGGING_FILENAME = "../Logs/cur_log.log";
  
    // file stream used to access/moify logging file
    static std::ofstream LOGGING_FILE;
  
    // strips the Windows style path from a filename
    extern constexpr const char* FILENAME(const char* path);
  }

#endif