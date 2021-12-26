add_executable(Masters main.cc FibbHeap.h)
target_link_libraries(Masters PRIVATE fmt::fmt-header-only)
set_target_properties(
    Masters
    PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )
