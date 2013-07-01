set(mainserv_SRCS
  ComServer.cpp
  ComEntity.cpp
  MainService.cpp
  DefaultComCodec.cpp
  RS485Codec.cpp
  )

add_executable(mainserv ${mainserv_SRCS})
target_link_libraries(mainserv utils port)
include_directories(.)
include_directories(..)
install(TARGETS mainserv DESTINATION bin)
