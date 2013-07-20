
#
file (GLOB SRC_HDR *.h)
file (GLOB SRC_IMP *.c)

#
add_library (libpfe5993 SHARED ${SRC_HDR} ${SRC_IMP})

