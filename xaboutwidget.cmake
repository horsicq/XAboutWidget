include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../XShortcuts/xshortcuts.cmake)

set(XABOUTWIDGET_SOURCES
    ${XSHORTCUTS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xaboutwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xaboutwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xaboutwidget.ui
)
