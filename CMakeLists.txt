project(xmltoflatlistconverter)
cmake_minimum_required(VERSION 2.8)


# Making a lib
set(XMLROFLATLISTCONVERTER_HPPS
    include/xmltoflatlistconverter/api.hpp
    include/xmltoflatlistconverter/test_xmltoflatlistconverter.hpp
    include/xmltoflatlistconverter/xmltoflatlistconverter.hpp
)


set(XMLROFLATLISTCONVERTER_SRCS
    src/xmltoflatlistconverter.cpp
    src/test_xmltoflatlistconverter.cpp
)


include_directories(include)



if(NOT ONSEMCOMMON_FOUND)
  find_package(onsemcommon)
endif()
if(NOT ONSEMTEXTTOSEMANTIC_FOUND)
  find_package(onsemtexttosemantic)
endif()
if(NOT ONSEMSEMANTICTOTEXT_FOUND)
  find_package(onsemsemantictotext)
endif()

add_library(xmltoflatlistconverter SHARED
  ${XMLROFLATLISTCONVERTER_HPPS}
  ${XMLROFLATLISTCONVERTER_SRCS}
)

include_directories(
  ${ONSEMCOMMON_INCLUDE_DIRS}
  ${ONSEMTEXTTOSEMANTIC_INCLUDE_DIRS}
  ${ONSEMSEMANTICTOTEXT_INCLUDE_DIRS}
)

target_link_libraries(xmltoflatlistconverter
  ${ONSEMCOMMON_LIBRARIES}
  ${ONSEMTEXTTOSEMANTIC_LIBRARIES}
  ${ONSEMSEMANTICTOTEXT_LIBRARIES}
)

install(TARGETS xmltoflatlistconverter DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)


