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

add_executable(backend_r1_act11a
  unit/r1/test_act11a.c
)

target_include_directories(backend_r1_act11a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r1_act11a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.act11a COMMAND backend_r1_act11a)

add_executable(backend_r1_act11b
  unit/r1/test_act11b.c
)

target_include_directories(backend_r1_act11b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r1_act11b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.act11b COMMAND backend_r1_act11b)

add_executable(backend_r1_act12a
  unit/r1/test_act12a.c
)

target_include_directories(backend_r1_act12a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r1_act12a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.act12a COMMAND backend_r1_act12a)

add_executable(backend_r1_act13c
  unit/r1/test_act13c.c
)

target_include_directories(backend_r1_act13c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r1_act13c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.act13c COMMAND backend_r1_act13c)

add_executable(backend_r1_kowasi1
  unit/r1/test_kowasi1.c
)

target_include_directories(backend_r1_kowasi1 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r1_kowasi1 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.kowasi1 COMMAND backend_r1_kowasi1)

add_executable(backend_r1_friend1
  unit/r1/test_friend1.c
)

target_include_directories(backend_r1_friend1 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r1_friend1 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.friend1 COMMAND backend_r1_friend1)

add_executable(backend_r1_movie1
  unit/r1/test_movie1.c
)

target_include_directories(backend_r1_movie1 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r1_movie1 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.movie1 COMMAND backend_r1_movie1)

add_executable(backend_r1_sikake
  unit/r1/test_sikake.c
)

target_include_directories(backend_r1_sikake PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_sikake PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.sikake COMMAND backend_r1_sikake)

add_executable(backend_r1_shoot1
  unit/r1/test_shoot1.c
)

target_include_directories(backend_r1_shoot1 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_shoot1 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.shoot1 COMMAND backend_r1_shoot1)

add_executable(backend_r1_col1a
  unit/r1/test_col1a.c
)

target_include_directories(backend_r1_col1a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_col1a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.col1a COMMAND backend_r1_col1a)

add_executable(backend_r1_col1b
  unit/r1/test_col1b.c
)

target_include_directories(backend_r1_col1b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_col1b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.col1b COMMAND backend_r1_col1b)

add_executable(backend_r1_col1c
  unit/r1/test_col1c.c
)

target_include_directories(backend_r1_col1c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_col1c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.col1c COMMAND backend_r1_col1c)

add_executable(backend_r1_col1d
  unit/r1/test_col1d.c
)

target_include_directories(backend_r1_col1d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_col1d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.col1d COMMAND backend_r1_col1d)

add_executable(backend_r1_coli1
  unit/r1/test_coli1.c
)

target_include_directories(backend_r1_coli1 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_coli1 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.coli1 COMMAND backend_r1_coli1)

add_executable(backend_r1_kuzure
  unit/r1/test_kuzure.c
)

target_include_directories(backend_r1_kuzure PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_kuzure PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.kuzure COMMAND backend_r1_kuzure)

add_executable(backend_r1_scr13c
  unit/r1/test_scr13c.c
)

target_include_directories(backend_r1_scr13c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_scr13c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.scr13c COMMAND backend_r1_scr13c)

add_executable(backend_r1_scr13d
  unit/r1/test_scr13d.c
)

target_include_directories(backend_r1_scr13d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_scr13d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.scr13d COMMAND backend_r1_scr13d)

add_executable(backend_r1_scr11d
  unit/r1/test_scr11d.c
)

target_include_directories(backend_r1_scr11d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests/backend
)

set_target_properties(backend_r1_scr11d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r1.scr11d COMMAND backend_r1_scr11d)

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

add_executable(backend_r7_playsp7
  unit/r7/test_playsp7.c
)

target_include_directories(backend_r7_playsp7 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_playsp7 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.playsp7 COMMAND backend_r7_playsp7)

add_executable(backend_r8_playsp8
  unit/r8/test_playsp8.c
)

target_include_directories(backend_r8_playsp8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_playsp8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.playsp8 COMMAND backend_r8_playsp8)

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

add_executable(backend_r4_wall1
  unit/r4/test_wall1.c
)

target_include_directories(backend_r4_wall1 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r4_wall1 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r4.wall1 COMMAND backend_r4_wall1)

add_executable(backend_r4_wall4
  unit/r4/test_wall4.c
)

target_include_directories(backend_r4_wall4 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r4_wall4 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r4.wall4 COMMAND backend_r4_wall4)

add_executable(backend_r4_screw_a
  unit/r4/test_screw_a.c
)

target_include_directories(backend_r4_screw_a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r4_screw_a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r4.screw_a COMMAND backend_r4_screw_a)

add_executable(backend_r4_screw_c
  unit/r4/test_screw_c.c
)

target_include_directories(backend_r4_screw_c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r4_screw_c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r4.screw_c COMMAND backend_r4_screw_c)

add_executable(backend_r4_scrchk4
  unit/r4/test_scrchk4.c
)

target_include_directories(backend_r4_scrchk4 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r4_scrchk4 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r4.scrchk4 COMMAND backend_r4_scrchk4)

add_executable(backend_r5_beltsw5
  unit/r5/test_beltsw5.c
)

target_include_directories(backend_r5_beltsw5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_beltsw5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.beltsw5 COMMAND backend_r5_beltsw5)

add_executable(backend_r5_act51a
  unit/r5/test_act51a.c
)

target_include_directories(backend_r5_act51a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_act51a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.act51a COMMAND backend_r5_act51a)

add_executable(backend_r5_act53
  unit/r5/test_act53.c
)

target_include_directories(backend_r5_act53 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_act53 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.act53 COMMAND backend_r5_act53)

add_executable(backend_r5_buranko5
  unit/r5/test_buranko5.c
)

target_include_directories(backend_r5_buranko5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_buranko5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.buranko5 COMMAND backend_r5_buranko5)

add_executable(backend_r5_denden
  unit/r5/test_denden.c
)

target_include_directories(backend_r5_denden PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_denden PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.denden COMMAND backend_r5_denden)

add_executable(backend_r5_et5
  unit/r5/test_et5.c
)

target_include_directories(backend_r5_et5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_et5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.et5 COMMAND backend_r5_et5)

add_executable(backend_r5_friend5
  unit/r5/test_friend5.c
)

target_include_directories(backend_r5_friend5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_friend5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.friend5 COMMAND backend_r5_friend5)

add_executable(backend_r5_hari5f
  unit/r5/test_hari5f.c
)

target_include_directories(backend_r5_hari5f PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_hari5f PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.hari5f COMMAND backend_r5_hari5f)

add_executable(backend_r5_harir5
  unit/r5/test_harir5.c
)

target_include_directories(backend_r5_harir5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_harir5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.harir5 COMMAND backend_r5_harir5)

add_executable(backend_r5_hasira5
  unit/r5/test_hasira5.c
)

target_include_directories(backend_r5_hasira5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_hasira5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.hasira5 COMMAND backend_r5_hasira5)

add_executable(backend_r5_hashi5
  unit/r5/test_hashi5.c
)

target_include_directories(backend_r5_hashi5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_hashi5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.hashi5 COMMAND backend_r5_hashi5)

add_executable(backend_r5_iwa5
  unit/r5/test_iwa5.c
)

target_include_directories(backend_r5_iwa5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_iwa5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.iwa5 COMMAND backend_r5_iwa5)

add_executable(backend_r5_iwa5roll
  unit/r5/test_iwa5roll.c
)

target_include_directories(backend_r5_iwa5roll PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_iwa5roll PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.iwa5roll COMMAND backend_r5_iwa5roll)

add_executable(backend_r5_iwa5wave
  unit/r5/test_iwa5wave.c
)

target_include_directories(backend_r5_iwa5wave PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_iwa5wave PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.iwa5wave COMMAND backend_r5_iwa5wave)

add_executable(backend_r5_kowasi5
  unit/r5/test_kowasi5.c
)

target_include_directories(backend_r5_kowasi5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_kowasi5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.kowasi5 COMMAND backend_r5_kowasi5)

add_executable(backend_r5_kuzure5
  unit/r5/test_kuzure5.c
)

target_include_directories(backend_r5_kuzure5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_kuzure5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.kuzure5 COMMAND backend_r5_kuzure5)

add_executable(backend_r5_movie5
  unit/r5/test_movie5.c
)

target_include_directories(backend_r5_movie5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_movie5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.movie5 COMMAND backend_r5_movie5)

add_executable(backend_r5_playsp5
  unit/r5/test_playsp5.c
)

target_include_directories(backend_r5_playsp5 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r5_playsp5 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r5.playsp5 COMMAND backend_r5_playsp5)

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

add_executable(backend_r6_act61a
  unit/r6/test_act61a.c
)

target_include_directories(backend_r6_act61a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_act61a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.act61a COMMAND backend_r6_act61a)

add_executable(backend_r6_act62a
  unit/r6/test_act62a.c
)

target_include_directories(backend_r6_act62a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_act62a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.act62a COMMAND backend_r6_act62a)

add_executable(backend_r6_act63
  unit/r6/test_act63.c
)

target_include_directories(backend_r6_act63 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_act63 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.act63 COMMAND backend_r6_act63)

add_executable(backend_r6_beem6
  unit/r6/test_beem6.c
)

target_include_directories(backend_r6_beem6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_beem6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.beem6 COMMAND backend_r6_beem6)

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

add_executable(backend_r6_tobira6
  unit/r6/test_tobira6.c
)

target_include_directories(backend_r6_tobira6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_tobira6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.tobira6 COMMAND backend_r6_tobira6)

add_executable(backend_r6_trap_r6
  unit/r6/test_trap_r6.c
)

target_include_directories(backend_r6_trap_r6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_trap_r6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.trap_r6 COMMAND backend_r6_trap_r6)

add_executable(backend_r6_udblk6
  unit/r6/test_udblk6.c
)

target_include_directories(backend_r6_udblk6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_udblk6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.udblk6 COMMAND backend_r6_udblk6)

add_executable(backend_r6_tobidai6
  unit/r6/test_tobidai6.c
)

target_include_directories(backend_r6_tobidai6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_tobidai6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.tobidai6 COMMAND backend_r6_tobidai6)

add_executable(backend_r6_piston6
  unit/r6/test_piston6.c
)

target_include_directories(backend_r6_piston6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_piston6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.piston6 COMMAND backend_r6_piston6)

add_executable(backend_r6_dair6
  unit/r6/test_dair6.c
)

target_include_directories(backend_r6_dair6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dair6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dair6 COMMAND backend_r6_dair6)

add_executable(backend_r6_movie6
  unit/r6/test_movie6.c
)

target_include_directories(backend_r6_movie6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_movie6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.movie6 COMMAND backend_r6_movie6)

add_executable(backend_r6_semi
  unit/r6/test_semi.c
)

target_include_directories(backend_r6_semi PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_semi PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.semi COMMAND backend_r6_semi)

add_executable(backend_r6_hachi6
  unit/r6/test_hachi6.c
)

target_include_directories(backend_r6_hachi6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_hachi6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.hachi6 COMMAND backend_r6_hachi6)

add_executable(backend_r6_minomusi
  unit/r6/test_minomusi.c
)

target_include_directories(backend_r6_minomusi PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_minomusi PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.minomusi COMMAND backend_r6_minomusi)

add_executable(backend_r6_friend6
  unit/r6/test_friend6.c
)

target_include_directories(backend_r6_friend6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_friend6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.friend6 COMMAND backend_r6_friend6)

add_executable(backend_r6_egg6
  unit/r6/test_egg6.c
)

target_include_directories(backend_r6_egg6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_egg6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.egg6 COMMAND backend_r6_egg6)

add_executable(backend_r6_togebl6b
  unit/r6/test_togebl6b.c
)

target_include_directories(backend_r6_togebl6b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_togebl6b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.togebl6b COMMAND backend_r6_togebl6b)

add_executable(backend_r6_togebl6a
  unit/r6/test_togebl6a.c
)

target_include_directories(backend_r6_togebl6a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_togebl6a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.togebl6a COMMAND backend_r6_togebl6a)

add_executable(backend_r6_togebl6d
  unit/r6/test_togebl6d.c
)

target_include_directories(backend_r6_togebl6d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_togebl6d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.togebl6d COMMAND backend_r6_togebl6d)

add_executable(backend_r6_kdai6
  unit/r6/test_kdai6.c
)

target_include_directories(backend_r6_kdai6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_kdai6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.kdai6 COMMAND backend_r6_kdai6)

add_executable(backend_r6_et6
  unit/r6/test_et6.c
)

target_include_directories(backend_r6_et6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_et6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.et6 COMMAND backend_r6_et6)

add_executable(backend_r6_col6a
  unit/r6/test_col6a.c
)

target_include_directories(backend_r6_col6a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_col6a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.col6a COMMAND backend_r6_col6a)

add_executable(backend_r6_col6b
  unit/r6/test_col6b.c
)

target_include_directories(backend_r6_col6b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_col6b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.col6b COMMAND backend_r6_col6b)

add_executable(backend_r6_col6c
  unit/r6/test_col6c.c
)

target_include_directories(backend_r6_col6c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_col6c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.col6c COMMAND backend_r6_col6c)

add_executable(backend_r6_col6d
  unit/r6/test_col6d.c
)

target_include_directories(backend_r6_col6d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_col6d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.col6d COMMAND backend_r6_col6d)

add_executable(backend_r6_coli6
  unit/r6/test_coli6.c
)

target_include_directories(backend_r6_coli6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_coli6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.coli6 COMMAND backend_r6_coli6)

add_executable(backend_r6_dev61a
  unit/r6/test_dev61a.c
)

target_include_directories(backend_r6_dev61a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev61a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev61a COMMAND backend_r6_dev61a)

add_executable(backend_r6_dev61b
  unit/r6/test_dev61b.c
)

target_include_directories(backend_r6_dev61b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev61b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev61b COMMAND backend_r6_dev61b)

add_executable(backend_r6_dev61c
  unit/r6/test_dev61c.c
)

target_include_directories(backend_r6_dev61c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev61c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev61c COMMAND backend_r6_dev61c)

add_executable(backend_r6_dev61d
  unit/r6/test_dev61d.c
)

target_include_directories(backend_r6_dev61d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev61d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev61d COMMAND backend_r6_dev61d)

add_executable(backend_r6_dev62a
  unit/r6/test_dev62a.c
)

target_include_directories(backend_r6_dev62a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev62a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev62a COMMAND backend_r6_dev62a)

add_executable(backend_r6_dev62b
  unit/r6/test_dev62b.c
)

target_include_directories(backend_r6_dev62b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev62b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev62b COMMAND backend_r6_dev62b)

add_executable(backend_r6_dev62c
  unit/r6/test_dev62c.c
)

target_include_directories(backend_r6_dev62c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev62c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev62c COMMAND backend_r6_dev62c)

add_executable(backend_r6_dev62d
  unit/r6/test_dev62d.c
)

target_include_directories(backend_r6_dev62d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev62d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev62d COMMAND backend_r6_dev62d)

add_executable(backend_r6_dev63c
  unit/r6/test_dev63c.c
)

target_include_directories(backend_r6_dev63c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev63c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev63c COMMAND backend_r6_dev63c)

add_executable(backend_r6_dev63d
  unit/r6/test_dev63d.c
)

target_include_directories(backend_r6_dev63d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_dev63d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.dev63d COMMAND backend_r6_dev63d)

add_executable(backend_r6_seesaw6
  unit/r6/test_seesaw6.c
)

target_include_directories(backend_r6_seesaw6 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r6_seesaw6 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r6.seesaw6 COMMAND backend_r6_seesaw6)

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

add_executable(backend_r7_wall73
  unit/r7/test_wall73.c
)

target_include_directories(backend_r7_wall73 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r7_wall73 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r7.wall73 COMMAND backend_r7_wall73)

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

add_executable(backend_r7_emie7
    unit/r7/test_emie7.c
)
target_include_directories(backend_r7_emie7 PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(backend_r7_emie7 PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)
add_test(NAME backend.r7.emie7 COMMAND backend_r7_emie7)

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

add_executable(backend_r8_friend8
  unit/r8/test_friend8.c
)

target_include_directories(backend_r8_friend8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_friend8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.friend8 COMMAND backend_r8_friend8)

add_executable(backend_r8_konbea83
  unit/r8/test_konbea83.c
)

target_include_directories(backend_r8_konbea83 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_konbea83 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.konbea83 COMMAND backend_r8_konbea83)

add_executable(backend_r8_konbea
  unit/r8/test_konbea.c
)

target_include_directories(backend_r8_konbea PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_konbea PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.konbea COMMAND backend_r8_konbea)

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

add_executable(backend_r8_haguruma
  unit/r8/test_haguruma.c
)

target_include_directories(backend_r8_haguruma PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_haguruma PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.haguruma COMMAND backend_r8_haguruma)

add_executable(backend_r8_piston
  unit/r8/test_piston.c
)

target_include_directories(backend_r8_piston PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_piston PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.piston COMMAND backend_r8_piston)

add_executable(backend_r8_kuzure8
  unit/r8/test_kuzure8.c
)

target_include_directories(backend_r8_kuzure8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_kuzure8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.kuzure8 COMMAND backend_r8_kuzure8)

add_executable(backend_r8_dai8
  unit/r8/test_dai8.c
)

target_include_directories(backend_r8_dai8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dai8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dai8 COMMAND backend_r8_dai8)

add_executable(backend_r8_dango8
  unit/r8/test_dango8.c
)

target_include_directories(backend_r8_dango8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_dango8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.dango8 COMMAND backend_r8_dango8)

add_executable(backend_r8_emie8
  unit/r8/test_emie8.c
)

target_include_directories(backend_r8_emie8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_emie8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.emie8 COMMAND backend_r8_emie8)

add_executable(backend_r8_pipe8
  unit/r8/test_pipe8.c
)

target_include_directories(backend_r8_pipe8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_pipe8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.pipe8 COMMAND backend_r8_pipe8)

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

add_executable(backend_r8_beam
  unit/r8/test_beam.c
)

target_include_directories(backend_r8_beam PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_beam PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.beam COMMAND backend_r8_beam)

add_executable(backend_r8_bigbom8
  unit/r8/test_bigbom8.c
)

target_include_directories(backend_r8_bigbom8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_bigbom8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.bigbom8 COMMAND backend_r8_bigbom8)

add_executable(backend_r8_hachi8
  unit/r8/test_hachi8.c
)

target_include_directories(backend_r8_hachi8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_hachi8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.hachi8 COMMAND backend_r8_hachi8)

add_executable(backend_r8_hota8c
  unit/r8/test_hota8c.c
)

target_include_directories(backend_r8_hota8c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_hota8c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.hota8c COMMAND backend_r8_hota8c)

add_executable(backend_r8_hota8d
  unit/r8/test_hota8d.c
)

target_include_directories(backend_r8_hota8d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_hota8d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.hota8d COMMAND backend_r8_hota8d)

add_executable(backend_r8_tobira
  unit/r8/test_tobira.c
)

target_include_directories(backend_r8_tobira PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_tobira PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.tobira COMMAND backend_r8_tobira)

add_executable(backend_r8_koma8
  unit/r8/test_koma8.c
)

target_include_directories(backend_r8_koma8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_koma8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.koma8 COMMAND backend_r8_koma8)

add_executable(backend_r8_kabuto8
  unit/r8/test_kabuto8.c
)

target_include_directories(backend_r8_kabuto8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_kabuto8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.kabuto8 COMMAND backend_r8_kabuto8)

add_executable(backend_r8_nokogiri
  unit/r8/test_nokogiri.c
)

target_include_directories(backend_r8_nokogiri PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_nokogiri PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.nokogiri COMMAND backend_r8_nokogiri)

add_executable(backend_r8_legmeca8
  unit/r8/test_legmeca8.c
)

target_include_directories(backend_r8_legmeca8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_legmeca8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.legmeca8 COMMAND backend_r8_legmeca8)

add_executable(backend_r8_propera8
  unit/r8/test_propera8.c
)

target_include_directories(backend_r8_propera8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_propera8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.propera8 COMMAND backend_r8_propera8)

add_executable(backend_r8_trap_r81
  unit/r8/test_trap_r81.c
)

target_include_directories(backend_r8_trap_r81 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_trap_r81 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.trap_r81 COMMAND backend_r8_trap_r81)

add_executable(backend_r8_trap_r82
  unit/r8/test_trap_r82.c
)

target_include_directories(backend_r8_trap_r82 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_trap_r82 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.trap_r82 COMMAND backend_r8_trap_r82)

add_executable(backend_r8_trap_r83
  unit/r8/test_trap_r83.c
)

target_include_directories(backend_r8_trap_r83 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_trap_r83 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.trap_r83 COMMAND backend_r8_trap_r83)

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

add_executable(backend_r8_col8a
  unit/r8/test_col8a.c
)

target_include_directories(backend_r8_col8a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_col8a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.col8a COMMAND backend_r8_col8a)

add_executable(backend_r8_col8b
  unit/r8/test_col8b.c
)

target_include_directories(backend_r8_col8b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_col8b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.col8b COMMAND backend_r8_col8b)

add_executable(backend_r8_col8c
  unit/r8/test_col8c.c
)

target_include_directories(backend_r8_col8c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_col8c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.col8c COMMAND backend_r8_col8c)

add_executable(backend_r8_col8d
  unit/r8/test_col8d.c
)

target_include_directories(backend_r8_col8d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_col8d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.col8d COMMAND backend_r8_col8d)

add_executable(backend_r8_coli8
  unit/r8/test_coli8.c
)

target_include_directories(backend_r8_coli8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_coli8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.coli8 COMMAND backend_r8_coli8)

add_executable(backend_r8_okusieso
  unit/r8/test_okusieso.c
)

target_include_directories(backend_r8_okusieso PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_okusieso PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.okusieso COMMAND backend_r8_okusieso)

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

add_executable(backend_r8_edtbl81
  unit/r8/test_edtbl81.c
)

target_include_directories(backend_r8_edtbl81 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_edtbl81 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.edtbl81 COMMAND backend_r8_edtbl81)

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

add_executable(backend_r8_edtbl83
  unit/r8/test_edtbl83.c
)

target_include_directories(backend_r8_edtbl83 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_edtbl83 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.edtbl83 COMMAND backend_r8_edtbl83)

add_executable(backend_r8_z81atbl
  unit/r8/test_z81atbl.c
)

target_include_directories(backend_r8_z81atbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z81atbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z81atbl COMMAND backend_r8_z81atbl)

add_executable(backend_r8_z81btbl
  unit/r8/test_z81btbl.c
)

target_include_directories(backend_r8_z81btbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z81btbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z81btbl COMMAND backend_r8_z81btbl)

add_executable(backend_r8_z81ctbl
  unit/r8/test_z81ctbl.c
)

target_include_directories(backend_r8_z81ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z81ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z81ctbl COMMAND backend_r8_z81ctbl)

add_executable(backend_r8_z81dtbl
  unit/r8/test_z81dtbl.c
)

target_include_directories(backend_r8_z81dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z81dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z81dtbl COMMAND backend_r8_z81dtbl)

add_executable(backend_r8_z82atbl
  unit/r8/test_z82atbl.c
)

target_include_directories(backend_r8_z82atbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z82atbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z82atbl COMMAND backend_r8_z82atbl)

add_executable(backend_r8_z82btbl
  unit/r8/test_z82btbl.c
)

target_include_directories(backend_r8_z82btbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z82btbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z82btbl COMMAND backend_r8_z82btbl)

add_executable(backend_r8_z82ctbl
  unit/r8/test_z82ctbl.c
)

target_include_directories(backend_r8_z82ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z82ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z82ctbl COMMAND backend_r8_z82ctbl)

add_executable(backend_r8_z82dtbl
  unit/r8/test_z82dtbl.c
)

target_include_directories(backend_r8_z82dtbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z82dtbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z82dtbl COMMAND backend_r8_z82dtbl)

add_executable(backend_r8_z83ctbl
  unit/r8/test_z83ctbl.c
)

target_include_directories(backend_r8_z83ctbl PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_z83ctbl PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.z83ctbl COMMAND backend_r8_z83ctbl)

add_executable(backend_r8_shut
  unit/r8/test_shut.c
)

target_include_directories(backend_r8_shut PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_shut PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.shut COMMAND backend_r8_shut)

add_executable(backend_r8_scrchk8
  unit/r8/test_scrchk8.c
)

target_include_directories(backend_r8_scrchk8 PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scrchk8 PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scrchk8 COMMAND backend_r8_scrchk8)

add_executable(backend_r8_scr81a
  unit/r8/test_scr81a.c
)

target_include_directories(backend_r8_scr81a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr81a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr81a COMMAND backend_r8_scr81a)

add_executable(backend_r8_scr81b
  unit/r8/test_scr81b.c
)

target_include_directories(backend_r8_scr81b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr81b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr81b COMMAND backend_r8_scr81b)

add_executable(backend_r8_scr81c
  unit/r8/test_scr81c.c
)

target_include_directories(backend_r8_scr81c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr81c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr81c COMMAND backend_r8_scr81c)

add_executable(backend_r8_scr81d
  unit/r8/test_scr81d.c
)

target_include_directories(backend_r8_scr81d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr81d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr81d COMMAND backend_r8_scr81d)

add_executable(backend_r8_scr82a
  unit/r8/test_scr82a.c
)

target_include_directories(backend_r8_scr82a PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr82a PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr82a COMMAND backend_r8_scr82a)

add_executable(backend_r8_scr82b
  unit/r8/test_scr82b.c
)

target_include_directories(backend_r8_scr82b PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr82b PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr82b COMMAND backend_r8_scr82b)

add_executable(backend_r8_scr82c
  unit/r8/test_scr82c.c
)

target_include_directories(backend_r8_scr82c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr82c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr82c COMMAND backend_r8_scr82c)

add_executable(backend_r8_scr82d
  unit/r8/test_scr82d.c
)

target_include_directories(backend_r8_scr82d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr82d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr82d COMMAND backend_r8_scr82d)

add_executable(backend_r8_scr83c
  unit/r8/test_scr83c.c
)

target_include_directories(backend_r8_scr83c PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr83c PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr83c COMMAND backend_r8_scr83c)

add_executable(backend_r8_scr83d
  unit/r8/test_scr83d.c
)

target_include_directories(backend_r8_scr83d PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_scr83d PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.scr83d COMMAND backend_r8_scr83d)

add_executable(backend_r8_shoot
  unit/r8/test_shoot.c
)

target_include_directories(backend_r8_shoot PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend_r8_shoot PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
)

add_test(NAME backend.r8.shoot COMMAND backend_r8_shoot)

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
    backend_r1_act11a
    backend_r1_act11b
    backend_r1_act12a
    backend_r1_act13c
    backend_r1_kowasi1
    backend_r1_friend1
    backend_r1_movie1
    backend_r1_sikake
    backend_r1_shoot1
    backend_r1_col1a
    backend_r1_col1b
    backend_r1_col1c
    backend_r1_col1d
    backend_r1_coli1
    backend_r1_kuzure
    backend_r1_scr13c
    backend_r1_scr13d
    backend_r1_scr11d
    backend_r3_playsp3
    backend_r7_playsp7
    backend_r8_playsp8
    backend_r5_harir5
    backend_r5_hasira5
    backend_r5_hashi5
    backend_r5_hari5f
    backend_r5_iwa5
    backend_r5_iwa5roll
    backend_r5_iwa5wave
    backend_r5_kowasi5
    backend_r5_kuzure5
    backend_r5_movie5
    backend_r5_playsp5
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
    backend_r4_wall1
    backend_r4_wall4
    backend_r4_screw_a
    backend_r4_screw_c
    backend_r4_scrchk4
    backend_r5_beltsw5
    backend_r5_act51a
    backend_r5_act53
    backend_r5_buranko5
    backend_r5_denden
    backend_r5_et5
    backend_r5_friend5
    backend_r6_batta
    backend_r6_act61a
    backend_r6_act62a
    backend_r6_act63
    backend_r6_beem6
    backend_r6_sw6
    backend_r6_tobira6
    backend_r6_trap_r6
    backend_r6_udblk6
    backend_r6_tobidai6
    backend_r6_piston6
    backend_r6_dair6
    backend_r6_movie6
    backend_r6_semi
    backend_r6_hachi6
    backend_r6_minomusi
    backend_r6_friend6
    backend_r6_egg6
    backend_r6_togebl6b
    backend_r6_togebl6a
    backend_r6_togebl6d
    backend_r6_kdai6
    backend_r6_et6
    backend_r6_col6a
    backend_r6_col6b
    backend_r6_col6c
    backend_r6_col6d
    backend_r6_coli6
    backend_r6_dev61a
    backend_r6_dev61b
    backend_r6_dev61c
    backend_r6_dev61d
    backend_r6_dev62a
    backend_r6_dev62b
    backend_r6_dev62c
    backend_r6_dev62d
    backend_r6_dev63c
    backend_r6_dev63d
    backend_r6_seesaw6
    backend_r7_chgwall7
    backend_r7_chgban
    backend_r7_hari73
    backend_r7_wall7
    backend_r7_wall73
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
    backend_r7_emie7
    backend_r7_dango7
    backend_r7_branko7
    backend_r7_slight7
    backend_r8_et8
    backend_r8_friend8
    backend_r8_konbea83
    backend_r8_konbea
    backend_r8_scarab
    backend_r8_sw8
    backend_r8_haguruma
    backend_r8_piston
    backend_r8_kuzure8
    backend_r8_dai8
    backend_r8_dango8
    backend_r8_emie8
    backend_r8_pipe8
    backend_r8_jettoge8
    backend_r8_beam
    backend_r8_bigbom8
    backend_r8_hachi8
    backend_r8_hota8c
    backend_r8_hota8d
    backend_r8_tobira
    backend_r8_koma8
    backend_r8_kabuto8
    backend_r8_nokogiri
    backend_r8_legmeca8
    backend_r8_propera8
    backend_r8_trap_r81
    backend_r8_trap_r82
    backend_r8_trap_r83
    backend_r8_act82a
    backend_r8_act81a
    backend_r8_act83
    backend_r8_col8a
    backend_r8_col8b
    backend_r8_col8c
    backend_r8_col8d
    backend_r8_coli8
    backend_r8_okusieso
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
    backend_r8_edtbl81
    backend_r8_edtbl82
    backend_r8_edtbl83
    backend_r8_z81atbl
    backend_r8_z81btbl
    backend_r8_z81ctbl
    backend_r8_z81dtbl
    backend_r8_z82atbl
    backend_r8_z82btbl
    backend_r8_z82ctbl
    backend_r8_z82dtbl
    backend_r8_z83ctbl
    backend_r8_shut
    backend_r8_scrchk8
    backend_r8_scr81a
    backend_r8_scr81b
    backend_r8_scr81c
    backend_r8_scr81d
    backend_r8_scr82a
    backend_r8_scr82b
    backend_r8_scr82c
    backend_r8_scr82d
    backend_r8_scr83c
    backend_r8_scr83d
    backend_r8_shoot
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
