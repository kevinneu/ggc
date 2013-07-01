set(port_SRCS
  Buffer.cpp
  EPollPoller.cpp
  EventLoopThreadPool.cpp
  Timer.cpp
  Channel.cpp
  EventLoop.cpp
  Poller.cpp
  TimerQueue.cpp
  DefaultPoller.cpp
  EventLoopThread.cpp
  PollPoller.cpp
  )

add_library(port ${port_SRCS})
target_link_libraries(port pthread rt)
install(TARGETS port DESTINATION lib)
