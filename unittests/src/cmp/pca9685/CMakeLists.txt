set(
        SRC_FILES
        ../src/Component/ServosController/Pca9685.cpp
)
set(
        TEST_FILE
        src/cmp/pca9685/UT_CMP_PCA9685.cpp
)
set(UT_CMP_PCA9685 ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_PCA9685} PARENT_SCOPE)