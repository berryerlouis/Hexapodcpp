cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Component/Adc/Ads1115.cpp
)
set(
        TEST_FILE
        src/cmp/ads1115/UT_CMP_ADS1115.cpp
)
set(UT_CMP_ADS1115 ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_ADS1115} PARENT_SCOPE)