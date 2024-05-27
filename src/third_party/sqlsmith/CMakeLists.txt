include_directories(include)

add_library(
  sqlsmith OBJECT
  duckdb.cc
  dump.cc
  expr.cc
  grammar.cc
  impedance.cc
  log.cc
  prod.cc
  random.cc
  relmodel.cc
  schema.cc
  sqlsmith.cc)
set(EXTENSION_OBJECT_FILES
    ${EXTENSION_OBJECT_FILES} $<TARGET_OBJECTS:sqlsmith>
    PARENT_SCOPE)

disable_target_warnings(sqlsmith)
