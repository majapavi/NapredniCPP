add_executable(fmtprog main.cpp)
target_link_libraries(fmtprog PRIVATE fmt::fmt-header-only)
set_target_properties(
	fmtprog
	PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS ON
	CXX_STANDARD_REQUIRED ON
)
