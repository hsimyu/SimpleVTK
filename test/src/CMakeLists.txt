set(Basic_SRCS "basicTest.cpp")
add_executable(BasicTest ${TEST_FIXTURE_SRC} ${Basic_SRCS})
target_link_libraries(BasicTest gtest gtest_main)
add_test(
    NAME BasicTest
    COMMAND $<TARGET_FILE:BasicTest>
)

set(ImageData_SRCS "vtkImageDataTest.cpp")
add_executable(ImageDataTest ${TEST_FIXTURE_SRC} ${ImageData_SRCS})
target_link_libraries(ImageDataTest gtest gtest_main)
add_test(
    NAME ImageDataTest
    COMMAND $<TARGET_FILE:ImageDataTest>
)

set(HierarchicalBoxDataSet_SRCS "vtkHierarchicalBoxDataSetTest.cpp")
add_executable(HierarchicalBoxDataSetTest ${TEST_FIXTURE_SRC} ${HierarchicalBoxDataSet_SRCS})
target_link_libraries(HierarchicalBoxDataSetTest gtest gtest_main)
add_test(
    NAME HierarchicalBoxDataSetTest
    COMMAND $<TARGET_FILE:HierarchicalBoxDataSetTest>
)