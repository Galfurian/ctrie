# Add the test.
add_executable(ctrie_test_ctrie test_ctrie.cpp)
# Inlcude header directories.
target_include_directories(ctrie_test_ctrie PUBLIC ${PROJECT_SOURCE_DIR}/include)
# Inlcude header directories.
target_link_libraries(ctrie_test_ctrie PUBLIC ctrie)
# Add the test.
add_test(NAME ctrie_run_test_ctrie COMMAND ctrie_test_ctrie)

# Add the test.
add_executable(ctrie_test_concurrency test_concurrency.cpp)
# Inlcude header directories.
target_include_directories(ctrie_test_concurrency PUBLIC ${PROJECT_SOURCE_DIR}/include)
# Inlcude header directories.
target_link_libraries(ctrie_test_concurrency PUBLIC ctrie pthread)
# Add the test.
add_test(NAME ctrie_run_test_concurrency COMMAND ctrie_test_concurrency)
