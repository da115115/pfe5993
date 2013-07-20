
#
set (MAIN_HDR maille.h)
set (MAIN_SRC maille.c)

#
add_executable (pfe5993 ${MAIN_HDR} ${MAIN_SRC})
add_dependencies (pfe5993 libpfe5993)

