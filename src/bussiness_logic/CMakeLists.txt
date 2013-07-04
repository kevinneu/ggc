set(mainserv_SRCS
  ComServer.cpp
  ComEntity.cpp
  MainService.cpp
  DefaultComCodec.cpp
  RS485Codec.cpp
  ConfigMgr.cpp
  MonitorUnitMgr.cpp
  MeasurePoint.cpp
  MonitorUnit.cpp
  )

add_executable(mainserv ${mainserv_SRCS})
target_link_libraries(mainserv utils port tinyxml)
include_directories(.)
include_directories(..)
install(TARGETS mainserv DESTINATION bin)
