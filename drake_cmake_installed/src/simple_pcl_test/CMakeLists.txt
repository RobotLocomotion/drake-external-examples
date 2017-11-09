find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
add_executable(simple_pcl_test simple_pcl_test.cc)
target_link_libraries(simple_pcl_test drake::drake pcl)
