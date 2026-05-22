add_executable(backend_smoke_dllmain
  unit/test_dllmain.c
)

target_include_directories(backend_smoke_dllmain PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_smoke_dllmain PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.smoke.dllmain COMMAND backend_smoke_dllmain)

add_executable(backend_action_spatset
  unit/test_action_spatset.c
  support/callback_log.c
)

target_include_directories(backend_action_spatset PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_action_spatset PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.action.spatset COMMAND backend_action_spatset)

add_executable(backend_actset
  unit/test_actset.c
  ${PROJECT_SOURCE_DIR}/src/actset.c
)

target_include_directories(backend_actset PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_actset PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.actset COMMAND backend_actset)

add_executable(backend_io
  unit/test_io.c
)

target_include_directories(backend_io PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_io PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.io COMMAND backend_io)

add_executable(backend_save
  unit/test_save.c
)

target_include_directories(backend_save PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_save PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.save COMMAND backend_save)

add_executable(backend_loader2
  unit/test_loader2.c
)

target_include_directories(backend_loader2 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_loader2 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.loader2 COMMAND backend_loader2)

add_executable(backend_scrchk
  unit/test_scrchk.c
)

target_include_directories(backend_scrchk PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_scrchk PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.scrchk COMMAND backend_scrchk)

add_executable(backend_score
  unit/test_score.c
)

target_include_directories(backend_score PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_score PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.score COMMAND backend_score)

add_executable(backend_ridechk
  unit/test_ridechk.c
)

target_include_directories(backend_ridechk PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_ridechk PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.ridechk COMMAND backend_ridechk)

add_executable(backend_zone
  unit/test_zone.c
)

target_include_directories(backend_zone PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_zone PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.zone COMMAND backend_zone)

add_executable(backend_dircol
  unit/test_dircol.c
)

target_include_directories(backend_dircol PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_dircol PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.dircol COMMAND backend_dircol)

add_executable(backend_fcol
  unit/test_fcol.c
)

target_include_directories(backend_fcol PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_fcol PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.fcol COMMAND backend_fcol)

add_executable(backend_bmp
  unit/test_bmp.c
)

target_include_directories(backend_bmp PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_bmp PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.bmp COMMAND backend_bmp)

add_executable(backend_block
  unit/test_block.c
)

target_include_directories(backend_block PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_block PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.block COMMAND backend_block)

add_executable(backend_suicide
  unit/test_suicide.c
)

target_include_directories(backend_suicide PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_suicide PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.suicide COMMAND backend_suicide)

add_executable(backend_taki
  unit/test_taki.c
)

target_include_directories(backend_taki PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_taki PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.taki COMMAND backend_taki)

add_executable(backend_plchg
  unit/test_plchg.c
)

target_include_directories(backend_plchg PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_plchg PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.plchg COMMAND backend_plchg)

add_executable(backend_plchg6
  unit/test_plchg6.c
)

target_include_directories(backend_plchg6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_plchg6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.plchg6 COMMAND backend_plchg6)

add_executable(backend_ring
  unit/test_ring.c
)

target_include_directories(backend_ring PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_ring PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.ring COMMAND backend_ring)

add_custom_target(backend-tests
  DEPENDS
    backend_smoke_dllmain
    backend_action_spatset
    backend_actset
    backend_io
    backend_save
    backend_loader2
    backend_scrchk
    backend_score
    backend_ridechk
    backend_zone
    backend_dircol
    backend_fcol
    backend_bmp
    backend_block
    backend_suicide
    backend_taki
    backend_plchg
    backend_plchg6
    backend_ring
)

find_program(SONICCD_POWERSHELL NAMES pwsh powershell)
if(SONICCD_POWERSHELL)
  add_custom_target(backend-source-inventory
    COMMAND ${SONICCD_POWERSHELL} -NoProfile -File
            ${CMAKE_CURRENT_SOURCE_DIR}/tools/inventory_backend_sources.ps1
            -RepoRoot ${PROJECT_SOURCE_DIR}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Inventory backend DLL sources"
    VERBATIM
  )
endif()
