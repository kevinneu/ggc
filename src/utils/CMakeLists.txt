set(utils_SRCS
    FileUtil.cpp
    TimeZone.cpp
    Date.cpp
    LogStream.cpp
    Timestamp.cpp
    Condition.cpp
    Logging.cpp
    ProcessInfo.cpp
    LogFile.cpp
    Thread.cpp
    Exception.cpp
    ThreadPool.cpp
    CountDownLatch.cpp
    AsyncLogging.cpp
  )

add_library(utils ${utils_SRCS})
target_link_libraries(utils pthread rt)
include_directories(../)
install(TARGETS utils DESTINATION lib)
