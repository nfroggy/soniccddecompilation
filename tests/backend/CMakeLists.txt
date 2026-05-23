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

add_executable(backend_playsp
  unit/test_playsp.c
)

target_include_directories(backend_playsp PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_playsp PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.playsp COMMAND backend_playsp)

add_executable(backend_playsub
  unit/test_playsub.c
)

target_include_directories(backend_playsub PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_playsub PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.playsub COMMAND backend_playsub)

add_executable(backend_item
  unit/test_item.c
)

target_include_directories(backend_item PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_item PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.item COMMAND backend_item)

add_executable(backend_enemy
  unit/test_enemy.c
)

target_include_directories(backend_enemy PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_enemy PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.enemy COMMAND backend_enemy)

add_executable(backend_spring
  unit/test_spring.c
)

target_include_directories(backend_spring PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_spring PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.spring COMMAND backend_spring)

add_executable(backend_goal
  unit/test_goal.c
)

target_include_directories(backend_goal PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_goal PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.goal COMMAND backend_goal)

add_executable(backend_etc
  unit/test_etc.c
)

target_include_directories(backend_etc PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_etc PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.etc COMMAND backend_etc)

add_executable(backend_dai_k
  unit/test_dai_k.c
)

target_include_directories(backend_dai_k PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_dai_k PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.dai_k COMMAND backend_dai_k)

add_executable(backend_r3_playsp3
  unit/r3/test_playsp3.c
)

target_include_directories(backend_r3_playsp3 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_playsp3 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.playsp3 COMMAND backend_r3_playsp3)

add_executable(backend_playsp7
  unit/test_playsp7.c
)

target_include_directories(backend_playsp7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_playsp7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.playsp7 COMMAND backend_playsp7)

add_executable(backend_playsp8
  unit/test_playsp8.c
)

target_include_directories(backend_playsp8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_playsp8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.playsp8 COMMAND backend_playsp8)

add_executable(backend_playsp5
  unit/test_playsp5.c
)

target_include_directories(backend_playsp5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_playsp5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.playsp5 COMMAND backend_playsp5)

add_executable(backend_r3_act31a
  unit/r3/test_act31a.c
)

target_include_directories(backend_r3_act31a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_act31a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.act31a COMMAND backend_r3_act31a)

add_executable(backend_r3_act31b
  unit/r3/test_act31b.c
)

target_include_directories(backend_r3_act31b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_act31b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.act31b COMMAND backend_r3_act31b)

add_executable(backend_r3_act32a
  unit/r3/test_act32a.c
)

target_include_directories(backend_r3_act32a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_act32a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.act32a COMMAND backend_r3_act32a)

add_executable(backend_r3_act33c
  unit/r3/test_act33c.c
)

target_include_directories(backend_r3_act33c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_act33c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.act33c COMMAND backend_r3_act33c)

add_executable(backend_r3_banpa
  unit/r3/test_banpa.c
)

target_include_directories(backend_r3_banpa PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_banpa PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.banpa COMMAND backend_r3_banpa)

add_executable(backend_r3_bobinb
  unit/r3/test_bobinb.c
)

target_include_directories(backend_r3_bobinb PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_bobinb PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.bobinb COMMAND backend_r3_bobinb)

add_executable(backend_r3_col3a
  unit/r3/test_col3a.c
)

target_include_directories(backend_r3_col3a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_col3a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.col3a COMMAND backend_r3_col3a)

add_executable(backend_r3_col3b
  unit/r3/test_col3b.c
)

target_include_directories(backend_r3_col3b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_col3b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.col3b COMMAND backend_r3_col3b)

add_executable(backend_r3_col3c
  unit/r3/test_col3c.c
)

target_include_directories(backend_r3_col3c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_col3c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.col3c COMMAND backend_r3_col3c)

add_executable(backend_r3_col3d
  unit/r3/test_col3d.c
)

target_include_directories(backend_r3_col3d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_col3d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.col3d COMMAND backend_r3_col3d)

add_executable(backend_r3_coli3
  unit/r3/test_coli3.c
)

target_include_directories(backend_r3_coli3 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_coli3 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.coli3 COMMAND backend_r3_coli3)

add_executable(backend_r3_bobin
  unit/r3/test_bobin.c
)

target_include_directories(backend_r3_bobin PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_bobin PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.bobin COMMAND backend_r3_bobin)

add_executable(backend_r3_dev31a
  unit/r3/test_dev31a.c
)

target_include_directories(backend_r3_dev31a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev31a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev31a COMMAND backend_r3_dev31a)

add_executable(backend_r3_dev31b
  unit/r3/test_dev31b.c
)

target_include_directories(backend_r3_dev31b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev31b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev31b COMMAND backend_r3_dev31b)

add_executable(backend_r3_dev31c
  unit/r3/test_dev31c.c
)

target_include_directories(backend_r3_dev31c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev31c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev31c COMMAND backend_r3_dev31c)

add_executable(backend_r3_dev31d
  unit/r3/test_dev31d.c
)

target_include_directories(backend_r3_dev31d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev31d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev31d COMMAND backend_r3_dev31d)

add_executable(backend_r3_dev32a
  unit/r3/test_dev32a.c
)

target_include_directories(backend_r3_dev32a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev32a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev32a COMMAND backend_r3_dev32a)

add_executable(backend_r3_dev32b
  unit/r3/test_dev32b.c
)

target_include_directories(backend_r3_dev32b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev32b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev32b COMMAND backend_r3_dev32b)

add_executable(backend_r3_dev32c
  unit/r3/test_dev32c.c
)

target_include_directories(backend_r3_dev32c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev32c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev32c COMMAND backend_r3_dev32c)

add_executable(backend_r3_dev32d
  unit/r3/test_dev32d.c
)

target_include_directories(backend_r3_dev32d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev32d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev32d COMMAND backend_r3_dev32d)

add_executable(backend_r3_dev33c
  unit/r3/test_dev33c.c
)

target_include_directories(backend_r3_dev33c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev33c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev33c COMMAND backend_r3_dev33c)

add_executable(backend_r3_dev33d
  unit/r3/test_dev33d.c
)

target_include_directories(backend_r3_dev33d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_dev33d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.dev33d COMMAND backend_r3_dev33d)

add_executable(backend_r3_edtbl31a
  unit/r3/test_edtbl31a.c
)

target_include_directories(backend_r3_edtbl31a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl31a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl31a COMMAND backend_r3_edtbl31a)

add_executable(backend_r3_edtbl31b
  unit/r3/test_edtbl31b.c
)

target_include_directories(backend_r3_edtbl31b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl31b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl31b COMMAND backend_r3_edtbl31b)

add_executable(backend_r3_edtbl31c
  unit/r3/test_edtbl31c.c
)

target_include_directories(backend_r3_edtbl31c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl31c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl31c COMMAND backend_r3_edtbl31c)

add_executable(backend_r3_edtbl31d
  unit/r3/test_edtbl31d.c
)

target_include_directories(backend_r3_edtbl31d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl31d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl31d COMMAND backend_r3_edtbl31d)

add_executable(backend_r3_edtbl32b
  unit/r3/test_edtbl32b.c
)

target_include_directories(backend_r3_edtbl32b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl32b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl32b COMMAND backend_r3_edtbl32b)

add_executable(backend_r3_edtbl32a
  unit/r3/test_edtbl32a.c
)

target_include_directories(backend_r3_edtbl32a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl32a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl32a COMMAND backend_r3_edtbl32a)

add_executable(backend_r3_edtbl32c
  unit/r3/test_edtbl32c.c
)

target_include_directories(backend_r3_edtbl32c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl32c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl32c COMMAND backend_r3_edtbl32c)

add_executable(backend_r3_edtbl32d
  unit/r3/test_edtbl32d.c
)

target_include_directories(backend_r3_edtbl32d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl32d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl32d COMMAND backend_r3_edtbl32d)

add_executable(backend_r3_edtbl33c
  unit/r3/test_edtbl33c.c
)

target_include_directories(backend_r3_edtbl33c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_edtbl33c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.edtbl33c COMMAND backend_r3_edtbl33c)

add_executable(backend_r3_et3
  unit/r3/test_et3.c
)

target_include_directories(backend_r3_et3 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_et3 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.et3 COMMAND backend_r3_et3)

add_executable(backend_r3_friend3
  unit/r3/test_friend3.c
)

target_include_directories(backend_r3_friend3 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_friend3 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.friend3 COMMAND backend_r3_friend3)

add_executable(backend_r3_ga3
  unit/r3/test_ga3.c
)

target_include_directories(backend_r3_ga3 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_ga3 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.ga3 COMMAND backend_r3_ga3)

add_executable(backend_r3_gate
  unit/r3/test_gate.c
)

target_include_directories(backend_r3_gate PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_gate PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.gate COMMAND backend_r3_gate)

add_executable(backend_r3_kama
  unit/r3/test_kama.c
)

target_include_directories(backend_r3_kama PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_kama PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.kama COMMAND backend_r3_kama)

add_executable(backend_r3_light
  unit/r3/test_light.c
)

target_include_directories(backend_r3_light PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_light PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.light COMMAND backend_r3_light)

add_executable(backend_r3_mecasnc
  unit/r3/test_mecasnc.c
)

target_include_directories(backend_r3_mecasnc PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_mecasnc PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.mecasnc COMMAND backend_r3_mecasnc)

add_executable(backend_r3_miracle
  unit/r3/test_miracle.c
)

target_include_directories(backend_r3_miracle PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_miracle PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.miracle COMMAND backend_r3_miracle)

add_executable(backend_r3_msnccg
  unit/r3/test_msnccg.c
)

target_include_directories(backend_r3_msnccg PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_msnccg PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.msnccg COMMAND backend_r3_msnccg)

add_executable(backend_r3_movie3
  unit/r3/test_movie3.c
)

target_include_directories(backend_r3_movie3 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_movie3 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.movie3 COMMAND backend_r3_movie3)

add_executable(backend_r3_pocket
  unit/r3/test_pocket.c
)

target_include_directories(backend_r3_pocket PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_pocket PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.pocket COMMAND backend_r3_pocket)

add_executable(backend_r3_rollplat
  unit/r3/test_rollplat.c
)

target_include_directories(backend_r3_rollplat PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_rollplat PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.rollplat COMMAND backend_r3_rollplat)

add_executable(backend_r3_sikake33
  unit/r3/test_sikake33.c
)

target_include_directories(backend_r3_sikake33 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_sikake33 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.sikake33 COMMAND backend_r3_sikake33)

add_executable(backend_r3_stopper
  unit/r3/test_stopper.c
)

target_include_directories(backend_r3_stopper PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_stopper PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.stopper COMMAND backend_r3_stopper)

add_executable(backend_r3_tentou
  unit/r3/test_tentou.c
)

target_include_directories(backend_r3_tentou PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_tentou PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.tentou COMMAND backend_r3_tentou)

add_executable(backend_r3_togebl3a
  unit/r3/test_togebl3a.c
)

target_include_directories(backend_r3_togebl3a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_togebl3a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.togebl3a COMMAND backend_r3_togebl3a)

add_executable(backend_r3_togebl3b
  unit/r3/test_togebl3b.c
)

target_include_directories(backend_r3_togebl3b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_togebl3b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.togebl3b COMMAND backend_r3_togebl3b)

add_executable(backend_r3_togebl3d
  unit/r3/test_togebl3d.c
)

target_include_directories(backend_r3_togebl3d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_togebl3d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.togebl3d COMMAND backend_r3_togebl3d)

add_executable(backend_r3_z31act
  unit/r3/test_z31act.c
)

target_include_directories(backend_r3_z31act PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z31act PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z31act COMMAND backend_r3_z31act)

add_executable(backend_r3_z31atbl
  unit/r3/test_z31atbl.c
)

target_include_directories(backend_r3_z31atbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z31atbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z31atbl COMMAND backend_r3_z31atbl)

add_executable(backend_r3_z31btbl
  unit/r3/test_z31btbl.c
)

target_include_directories(backend_r3_z31btbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z31btbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z31btbl COMMAND backend_r3_z31btbl)

add_executable(backend_r3_z31ctbl
  unit/r3/test_z31ctbl.c
)

target_include_directories(backend_r3_z31ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z31ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z31ctbl COMMAND backend_r3_z31ctbl)

add_executable(backend_r3_z31dtbl
  unit/r3/test_z31dtbl.c
)

target_include_directories(backend_r3_z31dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z31dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z31dtbl COMMAND backend_r3_z31dtbl)

add_executable(backend_r3_z32act
  unit/r3/test_z32act.c
)

target_include_directories(backend_r3_z32act PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z32act PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z32act COMMAND backend_r3_z32act)

add_executable(backend_r3_z32atbl
  unit/r3/test_z32atbl.c
)

target_include_directories(backend_r3_z32atbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z32atbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z32atbl COMMAND backend_r3_z32atbl)

add_executable(backend_r3_z32btbl
  unit/r3/test_z32btbl.c
)

target_include_directories(backend_r3_z32btbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z32btbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z32btbl COMMAND backend_r3_z32btbl)

add_executable(backend_r3_z32ctbl
  unit/r3/test_z32ctbl.c
)

target_include_directories(backend_r3_z32ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z32ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z32ctbl COMMAND backend_r3_z32ctbl)

add_executable(backend_r3_z32dtbl
  unit/r3/test_z32dtbl.c
)

target_include_directories(backend_r3_z32dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z32dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z32dtbl COMMAND backend_r3_z32dtbl)

add_executable(backend_r3_z33act
  unit/r3/test_z33act.c
)

target_include_directories(backend_r3_z33act PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z33act PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z33act COMMAND backend_r3_z33act)

add_executable(backend_r3_z33ctbl
  unit/r3/test_z33ctbl.c
)

target_include_directories(backend_r3_z33ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z33ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z33ctbl COMMAND backend_r3_z33ctbl)

add_executable(backend_r3_z33dtbl
  unit/r3/test_z33dtbl.c
)

target_include_directories(backend_r3_z33dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_z33dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.z33dtbl COMMAND backend_r3_z33dtbl)

add_executable(backend_r3_zonetbl3
  unit/r3/test_zonetbl3.c
)

target_include_directories(backend_r3_zonetbl3 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r3_zonetbl3 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r3.zonetbl3 COMMAND backend_r3_zonetbl3)

add_executable(backend_r4_rblk4
  unit/r4/test_rblk4.c
)

target_include_directories(backend_r4_rblk4 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r4_rblk4 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r4.rblk4 COMMAND backend_r4_rblk4)

add_executable(backend_r6_batta
  unit/r6/test_batta.c
)

target_include_directories(backend_r6_batta PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_batta PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.batta COMMAND backend_r6_batta)

add_executable(backend_r6_sw6
  unit/r6/test_sw6.c
)

target_include_directories(backend_r6_sw6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_sw6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.sw6 COMMAND backend_r6_sw6)

add_executable(backend_r7_chgwall7
  unit/r7/test_chgwall7.c
)

target_include_directories(backend_r7_chgwall7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_chgwall7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.chgwall7 COMMAND backend_r7_chgwall7)

add_executable(backend_r7_chgban
  unit/r7/test_chgban.c
)

target_include_directories(backend_r7_chgban PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_chgban PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.chgban COMMAND backend_r7_chgban)

add_executable(backend_r7_hari73
  unit/r7/test_hari73.c
)

target_include_directories(backend_r7_hari73 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_hari73 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.hari73 COMMAND backend_r7_hari73)

add_executable(backend_r7_wall7
  unit/r7/test_wall7.c
)

target_include_directories(backend_r7_wall7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_wall7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.wall7 COMMAND backend_r7_wall7)

add_executable(backend_r7_et7
  unit/r7/test_et7.c
)

target_include_directories(backend_r7_et7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_et7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.et7 COMMAND backend_r7_et7)

add_executable(backend_r7_movie7
  unit/r7/test_movie7.c
)

target_include_directories(backend_r7_movie7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_movie7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.movie7 COMMAND backend_r7_movie7)

add_executable(backend_r7_friend7
  unit/r7/test_friend7.c
)

target_include_directories(backend_r7_friend7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_friend7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.friend7 COMMAND backend_r7_friend7)

add_executable(backend_r7_kanabun
  unit/r7/test_kanabun.c
)

target_include_directories(backend_r7_kanabun PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_kanabun PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.kanabun COMMAND backend_r7_kanabun)

add_executable(backend_r7_tekkyu7
  unit/r7/test_tekkyu7.c
)

target_include_directories(backend_r7_tekkyu7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_tekkyu7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.tekkyu7 COMMAND backend_r7_tekkyu7)

add_executable(backend_r7_tekkyu7j
  unit/r7/test_tekkyu7j.c
)

target_include_directories(backend_r7_tekkyu7j PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_tekkyu7j PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.tekkyu7j COMMAND backend_r7_tekkyu7j)

add_executable(backend_r7_gaitou73
  unit/r7/test_gaitou73.c
)

target_include_directories(backend_r7_gaitou73 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_gaitou73 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.gaitou73 COMMAND backend_r7_gaitou73)

add_executable(backend_r7_act73
  unit/r7/test_act73.c
)

target_include_directories(backend_r7_act73 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_act73 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.act73 COMMAND backend_r7_act73)

add_executable(backend_r7_act71a
  unit/r7/test_act71a.c
)

target_include_directories(backend_r7_act71a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_act71a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.act71a COMMAND backend_r7_act71a)

add_executable(backend_r7_z73act
  unit/r7/test_z73act.c
)

target_include_directories(backend_r7_z73act PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z73act PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z73act COMMAND backend_r7_z73act)

add_executable(backend_r7_z72act
  unit/r7/test_z72act.c
)

target_include_directories(backend_r7_z72act PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z72act PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z72act COMMAND backend_r7_z72act)

add_executable(backend_r7_z71act
  unit/r7/test_z71act.c
)

target_include_directories(backend_r7_z71act PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z71act PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z71act COMMAND backend_r7_z71act)

add_executable(backend_r7_z73ctbl
  unit/r7/test_z73ctbl.c
)

target_include_directories(backend_r7_z73ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z73ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z73ctbl COMMAND backend_r7_z73ctbl)

add_executable(backend_r7_z73dtbl
  unit/r7/test_z73dtbl.c
)

target_include_directories(backend_r7_z73dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z73dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z73dtbl COMMAND backend_r7_z73dtbl)

add_executable(backend_r7_zonetbl7
  unit/r7/test_zonetbl7.c
)

target_include_directories(backend_r7_zonetbl7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_zonetbl7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.zonetbl7 COMMAND backend_r7_zonetbl7)

add_executable(backend_r7_edtbl7
  unit/r7/test_edtbl7.c
)

target_include_directories(backend_r7_edtbl7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_edtbl7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.edtbl7 COMMAND backend_r7_edtbl7)

add_executable(backend_r7_edtbl73
  unit/r7/test_edtbl73.c
)

target_include_directories(backend_r7_edtbl73 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_edtbl73 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.edtbl73 COMMAND backend_r7_edtbl73)

add_executable(backend_r7_z72ctbl
  unit/r7/test_z72ctbl.c
)

target_include_directories(backend_r7_z72ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z72ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z72ctbl COMMAND backend_r7_z72ctbl)

add_executable(backend_r7_z71ctbl
  unit/r7/test_z71ctbl.c
)

target_include_directories(backend_r7_z71ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z71ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z71ctbl COMMAND backend_r7_z71ctbl)

add_executable(backend_r7_z71atbl
  unit/r7/test_z71atbl.c
)

target_include_directories(backend_r7_z71atbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z71atbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z71atbl COMMAND backend_r7_z71atbl)

add_executable(backend_r7_z71btbl
  unit/r7/test_z71btbl.c
)

target_include_directories(backend_r7_z71btbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z71btbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z71btbl COMMAND backend_r7_z71btbl)

add_executable(backend_r7_z71dtbl
  unit/r7/test_z71dtbl.c
)

target_include_directories(backend_r7_z71dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z71dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z71dtbl COMMAND backend_r7_z71dtbl)

add_executable(backend_r7_z72atbl
  unit/r7/test_z72atbl.c
)

target_include_directories(backend_r7_z72atbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z72atbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z72atbl COMMAND backend_r7_z72atbl)

add_executable(backend_r7_z72btbl
  unit/r7/test_z72btbl.c
)

target_include_directories(backend_r7_z72btbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z72btbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z72btbl COMMAND backend_r7_z72btbl)

add_executable(backend_r7_z72dtbl
  unit/r7/test_z72dtbl.c
)

target_include_directories(backend_r7_z72dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_z72dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.z72dtbl COMMAND backend_r7_z72dtbl)

add_executable(backend_r7_col7a
  unit/r7/test_col7a.c
)

target_include_directories(backend_r7_col7a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_col7a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.col7a COMMAND backend_r7_col7a)

add_executable(backend_r7_col7b
  unit/r7/test_col7b.c
)

target_include_directories(backend_r7_col7b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_col7b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.col7b COMMAND backend_r7_col7b)

add_executable(backend_r7_col7c
  unit/r7/test_col7c.c
)

target_include_directories(backend_r7_col7c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_col7c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.col7c COMMAND backend_r7_col7c)

add_executable(backend_r7_col7d
  unit/r7/test_col7d.c
)

target_include_directories(backend_r7_col7d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_col7d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.col7d COMMAND backend_r7_col7d)

add_executable(backend_r7_coli7
  unit/r7/test_coli7.c
)

target_include_directories(backend_r7_coli7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_coli7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.coli7 COMMAND backend_r7_coli7)

add_executable(backend_r7_dev71a
  unit/r7/test_dev71a.c
)

target_include_directories(backend_r7_dev71a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_dev71a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.dev71a COMMAND backend_r7_dev71a)

add_executable(backend_r7_dev71b
  unit/r7/test_dev71b.c
)

target_include_directories(backend_r7_dev71b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_dev71b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.dev71b COMMAND backend_r7_dev71b)

add_executable(backend_r7_dev71c
  unit/r7/test_dev71c.c
)

target_include_directories(backend_r7_dev71c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_dev71c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.dev71c COMMAND backend_r7_dev71c)

add_executable(backend_r7_dev71d
    unit/r7/test_dev71d.c
)
target_include_directories(backend_r7_dev71d PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dev71d PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dev71d COMMAND backend_r7_dev71d)

add_executable(backend_r7_dev72a
    unit/r7/test_dev72a.c
)
target_include_directories(backend_r7_dev72a PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dev72a PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dev72a COMMAND backend_r7_dev72a)

add_executable(backend_r7_dev72b
    unit/r7/test_dev72b.c
)
target_include_directories(backend_r7_dev72b PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dev72b PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dev72b COMMAND backend_r7_dev72b)

add_executable(backend_r7_dev72c
    unit/r7/test_dev72c.c
)
target_include_directories(backend_r7_dev72c PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dev72c PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dev72c COMMAND backend_r7_dev72c)

add_executable(backend_r7_dev72d
    unit/r7/test_dev72d.c
)
target_include_directories(backend_r7_dev72d PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dev72d PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dev72d COMMAND backend_r7_dev72d)

add_executable(backend_r7_dev73c
    unit/r7/test_dev73c.c
)
target_include_directories(backend_r7_dev73c PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dev73c PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dev73c COMMAND backend_r7_dev73c)

add_executable(backend_r7_dev73d
    unit/r7/test_dev73d.c
)
target_include_directories(backend_r7_dev73d PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dev73d PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dev73d COMMAND backend_r7_dev73d)

add_executable(backend_r7_emie7cg
    unit/r7/test_emie7cg.c
)
target_include_directories(backend_r7_emie7cg PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_emie7cg PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.emie7cg COMMAND backend_r7_emie7cg)

add_executable(backend_r7_dango7
    unit/r7/test_dango7.c
)
target_include_directories(backend_r7_dango7 PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_dango7 PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.dango7 COMMAND backend_r7_dango7)

add_executable(backend_r7_branko7
    unit/r7/test_branko7.c
)

target_include_directories(backend_r7_branko7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_branko7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.branko7 COMMAND backend_r7_branko7)

add_executable(backend_r7_slight7
  unit/r7/test_slight7.c
)

target_include_directories(backend_r7_slight7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_slight7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.slight7 COMMAND backend_r7_slight7)

add_executable(backend_r8_et8
  unit/r8/test_et8.c
)

target_include_directories(backend_r8_et8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_et8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.et8 COMMAND backend_r8_et8)

add_executable(backend_r8_scarab
  unit/r8/test_scarab.c
)

target_include_directories(backend_r8_scarab PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scarab PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scarab COMMAND backend_r8_scarab)

add_executable(backend_r8_sw8
  unit/r8/test_sw8.c
)

target_include_directories(backend_r8_sw8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_sw8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.sw8 COMMAND backend_r8_sw8)

add_executable(backend_r8_jettoge8
  unit/r8/test_jettoge8.c
)

target_include_directories(backend_r8_jettoge8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_jettoge8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.jettoge8 COMMAND backend_r8_jettoge8)

add_executable(backend_r8_act82a
  unit/r8/test_act82a.c
)

target_include_directories(backend_r8_act82a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_act82a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.act82a COMMAND backend_r8_act82a)

add_executable(backend_r8_act81a
  unit/r8/test_act81a.c
)

target_include_directories(backend_r8_act81a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_act81a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.act81a COMMAND backend_r8_act81a)

add_executable(backend_r8_act83
  unit/r8/test_act83.c
)

target_include_directories(backend_r8_act83 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_act83 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.act83 COMMAND backend_r8_act83)

add_executable(backend_r8_dev82b
  unit/r8/test_dev82b.c
)

target_include_directories(backend_r8_dev82b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev82b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev82b COMMAND backend_r8_dev82b)

add_executable(backend_r8_dev82a
  unit/r8/test_dev82a.c
)

target_include_directories(backend_r8_dev82a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev82a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev82a COMMAND backend_r8_dev82a)

add_executable(backend_r8_dev82c
  unit/r8/test_dev82c.c
)

target_include_directories(backend_r8_dev82c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev82c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev82c COMMAND backend_r8_dev82c)

add_executable(backend_r8_dev82d
  unit/r8/test_dev82d.c
)

target_include_directories(backend_r8_dev82d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev82d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev82d COMMAND backend_r8_dev82d)

add_executable(backend_r8_dev81a
  unit/r8/test_dev81a.c
)

target_include_directories(backend_r8_dev81a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev81a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev81a COMMAND backend_r8_dev81a)

add_executable(backend_r8_dev81b
  unit/r8/test_dev81b.c
)

target_include_directories(backend_r8_dev81b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev81b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev81b COMMAND backend_r8_dev81b)

add_executable(backend_r8_dev81c
  unit/r8/test_dev81c.c
)

target_include_directories(backend_r8_dev81c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev81c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev81c COMMAND backend_r8_dev81c)

add_executable(backend_r8_dev81d
  unit/r8/test_dev81d.c
)

target_include_directories(backend_r8_dev81d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev81d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev81d COMMAND backend_r8_dev81d)

add_executable(backend_r8_dev83c
  unit/r8/test_dev83c.c
)

target_include_directories(backend_r8_dev83c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev83c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev83c COMMAND backend_r8_dev83c)

add_executable(backend_r8_dev83d
  unit/r8/test_dev83d.c
)

target_include_directories(backend_r8_dev83d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dev83d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dev83d COMMAND backend_r8_dev83d)

add_executable(backend_r8_edtbl82
  unit/r8/test_edtbl82.c
)

target_include_directories(backend_r8_edtbl82 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_edtbl82 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.edtbl82 COMMAND backend_r8_edtbl82)

add_executable(backend_player
  unit/test_player.c
  ${PROJECT_SOURCE_DIR}/src/equ.c
  ${PROJECT_SOURCE_DIR}/src/plchg.c
  ${PROJECT_SOURCE_DIR}/src/player.c
)

target_include_directories(backend_player PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_player PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.player COMMAND backend_player)

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
    backend_playsp
    backend_playsub
    backend_item
    backend_enemy
    backend_spring
    backend_goal
    backend_etc
    backend_dai_k
    backend_r3_playsp3
    backend_playsp7
    backend_playsp8
    backend_playsp5
    backend_r3_act31a
    backend_r3_act31b
    backend_r3_act32a
    backend_r3_act33c
    backend_r3_banpa
    backend_r3_bobin
    backend_r3_bobinb
    backend_r3_col3a
    backend_r3_col3b
    backend_r3_col3c
    backend_r3_col3d
    backend_r3_coli3
    backend_r3_dev31a
    backend_r3_dev31b
    backend_r3_dev31c
    backend_r3_dev31d
    backend_r3_dev32a
    backend_r3_dev32b
    backend_r3_dev32c
    backend_r3_dev32d
    backend_r3_dev33c
    backend_r3_dev33d
    backend_r3_edtbl31a
    backend_r3_edtbl31b
    backend_r3_edtbl31c
    backend_r3_edtbl31d
    backend_r3_edtbl32a
    backend_r3_edtbl32b
    backend_r3_edtbl32c
    backend_r3_edtbl32d
    backend_r3_edtbl33c
    backend_r3_et3
    backend_r3_friend3
    backend_r3_ga3
    backend_r3_gate
    backend_r3_kama
    backend_r3_light
    backend_r3_mecasnc
    backend_r3_miracle
    backend_r3_msnccg
    backend_r3_movie3
    backend_r3_pocket
    backend_r3_rollplat
    backend_r3_sikake33
    backend_r3_stopper
    backend_r3_tentou
    backend_r3_togebl3a
    backend_r3_togebl3b
    backend_r3_togebl3d
    backend_r3_z31act
    backend_r3_z31atbl
    backend_r3_z31btbl
    backend_r3_z31ctbl
    backend_r3_z31dtbl
    backend_r3_z32act
    backend_r3_z32atbl
    backend_r3_z32btbl
    backend_r3_z32ctbl
    backend_r3_z32dtbl
    backend_r3_z33act
    backend_r3_z33ctbl
    backend_r3_z33dtbl
    backend_r3_zonetbl3
    backend_r4_rblk4
    backend_r6_batta
    backend_r6_sw6
    backend_r7_chgwall7
    backend_r7_chgban
    backend_r7_hari73
    backend_r7_wall7
    backend_r7_et7
    backend_r7_movie7
    backend_r7_friend7
    backend_r7_kanabun
    backend_r7_tekkyu7
    backend_r7_tekkyu7j
    backend_r7_gaitou73
    backend_r7_act73
    backend_r7_act71a
    backend_r7_z73act
    backend_r7_z72act
    backend_r7_z71act
    backend_r7_z73ctbl
    backend_r7_z73dtbl
    backend_r7_zonetbl7
    backend_r7_edtbl7
    backend_r7_edtbl73
    backend_r7_z72ctbl
    backend_r7_z71ctbl
    backend_r7_z71atbl
    backend_r7_z71btbl
    backend_r7_z71dtbl
    backend_r7_z72atbl
    backend_r7_z72btbl
    backend_r7_z72dtbl
    backend_r7_col7a
    backend_r7_col7b
    backend_r7_col7c
    backend_r7_col7d
    backend_r7_coli7
    backend_r7_dev71a
    backend_r7_dev71b
    backend_r7_dev71c
    backend_r7_dev71d
    backend_r7_dev72a
    backend_r7_dev72b
    backend_r7_dev72c
    backend_r7_dev72d
    backend_r7_dev73c
    backend_r7_dev73d
    backend_r7_emie7cg
    backend_r7_dango7
    backend_r7_branko7
    backend_r7_slight7
    backend_r8_et8
    backend_r8_scarab
    backend_r8_sw8
    backend_r8_jettoge8
    backend_r8_act82a
    backend_r8_act81a
    backend_r8_act83
    backend_r8_dev82b
    backend_r8_dev82a
    backend_r8_dev82c
    backend_r8_dev82d
    backend_r8_dev81a
    backend_r8_dev81b
    backend_r8_dev81c
    backend_r8_dev81d
    backend_r8_dev83c
    backend_r8_dev83d
    backend_r8_edtbl82
    backend_player
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
