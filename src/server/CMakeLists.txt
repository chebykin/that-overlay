cmake_minimum_required (VERSION 2.8)

include (CodeGenerator)

generate_code (
  MODELS ${CMAKE_CURRENT_SOURCE_DIR}/interface
  SERVER_STUB_DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/implementation/objects
)

include_directories(
  ${PANGOCAIRO_INCLUDE_DIRS}
)

target_link_libraries(kmsthatoverlayimpl
  ${PANGOCAIRO_LIBRARIES}
  ${GSTREAMER_VIDEO_LIBRARIES}
)

