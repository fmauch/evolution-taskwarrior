list(APPEND SOURCES
  task_module.c
  m-mail-ui.h
  m-mail-ui.c
  m-shell-view-extension.h
  m-shell-view-extension.c
  m-utils.h
  m-utils.c
  )

add_library(taskwarrior-module MODULE ${SOURCES})

target_compile_definitions(taskwarrior-module PRIVATE
  G_LOG_DOMAIN=\"taskwarrior-module\"
  )

target_compile_options(taskwarrior-module PUBLIC
  ${EVOLUTION_SHELL_CFLAGS}
  ${EVOLUTION_CALENDAR_CFLAGS}
  ${EVOLUTION_MAIL_CFLAGS}
  ${LIBECAL_CFLAGS}
  )

target_include_directories(taskwarrior-module PUBLIC
  ${EVOLUTION_SHELL_INCLUDE_DIRS}
  ${EVOLUTION_CALENDAR_INCLUDE_DIRS}
  ${EVOLUTION_MAIL_INCLUDE_DIRS}
  ${LIBECAL_INCLUDE_DIRS}
  )

target_link_libraries(taskwarrior-module
  ${EVOLUTION_SHELL_LDFLAGS}
  ${EVOLUTION_CALENDAR_LDFLAGS}
  ${EVOLUTION_MAIL_LDFLAGS}
  ${LIBECAL_LDFLAGS}
  )

install(TARGETS taskwarrior-module
  DESTINATION ${EVOLUTION_MODULE_DIR}
  )
