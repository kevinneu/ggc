add_subdirectory(utils)
add_subdirectory(unittest)

set(mainserv_SRCS
  Channel.cpp
  EventLoop.cpp
  Poller.cpp
  MainService.cpp
  TimerQueue.cpp
  Timer.cpp
  DefaultPoller.cpp
  EPollPoller.cpp
  PollPoller.cpp
  Buffer.cpp
  EventLoopThreadPool.cpp
  EventLoopThread.cpp
  ComEntity.cpp
  ComServer.cpp
  )

add_executable(mainserv ${mainserv_SRCS})
target_link_libraries(mainserv utils)
include_directories(.)
include_directories(..)
install(TARGETS mainserv DESTINATION bin)

