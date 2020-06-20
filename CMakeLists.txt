cmake_minimum_required(VERSION 3.16)
project(BedrockFinderCpp)

find_package(wxWidgets REQUIRED COMPONENTS core)
include(${wxWidgets_USE_FILE})

find_package(OpenCL REQUIRED)

add_executable(${PROJECT_NAME}
	BFFApp.cpp
	BFFGui.cpp
	PrecomputedRandAdvance.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OpenCL_LIBRARIES})
