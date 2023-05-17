add_test([=[TestEmpty.TestEmpty]=]  C:/Progetto_PCS/PCS2023_Exercises/Projects/Debug/raffinamento_test.exe [==[--gtest_filter=TestEmpty.TestEmpty]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TestEmpty.TestEmpty]=]  PROPERTIES WORKING_DIRECTORY C:/Progetto_PCS/PCS2023_Exercises/Projects/Debug SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  raffinamento_test_TESTS TestEmpty.TestEmpty)
