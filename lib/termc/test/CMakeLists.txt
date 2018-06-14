#
# MIT License
#
# Copyright (c) 2016-2018 Abel Lucas <www.github.com/uael>
# Copyright (c) 2016-2018 Guts <www.github.com/guts-lang>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

add_custom_target(check_termc
  COMMAND ${CMAKE_CTEST_COMMAND} -R '^test_termc_.*' --timeout 2)

macro (termc_register_test suite test)
  if (NOT TARGET check_termc_${suite})
    add_custom_target(check_termc_${suite}
      COMMAND ${CMAKE_CTEST_COMMAND} -R '^test_termc_${suite}_.*' --timeout 2)
  endif ()

  set(name ${suite}_${test})

  add_executable(test_termc_${name} ${name}.c test.h)
  target_link_libraries(test_termc_${name} termc)

  add_test(test_termc_${name} test_termc_${name})
  add_dependencies(check test_termc_${name})
  add_dependencies(check_termc test_termc_${name})
  add_dependencies(check_termc_${suite} test_termc_${name})
  add_dependencies(test_termc_${name} termc)
endmacro ()

termc_register_test(term set)
