set(FUCNTIONS_DIR "functions")
set(ASCII_DIR "ascii")
set(RTU_DIR "rtu")
set(TCP_DIR "tcp")
set(PORT_DIR "port")

set(libmodbus_SRCS
  mb.c
  ascii/mbascii.c
  functions/mbfunccoils.c
  functions/mbfuncdisc.c
  functions/mbfuncinput.c
  functions/mbutils.c
  functions/mbfuncdiag.c
  functions/mbfuncholding.c
  functions/mbfuncother.c
  rtu/mbcrc.c
  rtu/mbrtu.c
  tcp/mbtcp.c
  port/portevent.c
  port/portother.c
  port/portserial.c
  port/porttimer.c
  )

include_directories(${FUCNTIONS_DIR})
include_directories(${ASCII_DIR})
include_directories(${RTU_DIR})
include_directories(${TCP_DIR})
include_directories(${PORT_DIR})
include_directories(include)

add_library(modbus ${libmodbus_SRCS})
target_link_libraries(modbus pthread rt)

install(TARGETS modbus DESTINATION lib)
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/modbus)
