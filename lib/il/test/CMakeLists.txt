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

add_custom_target(check_il
  COMMAND ${CMAKE_CTEST_COMMAND} -R '^test_il_.*' --timeout 2)

macro (il_register_test suite test)
  if (NOT TARGET check_il_${suite})
    add_custom_target(check_il_${suite}
      COMMAND ${CMAKE_CTEST_COMMAND} -R '^test_il_${suite}_.*' --timeout 2)
  endif ()

  set(name ${suite}_${test})

  add_executable(test_il_${name} ${name}.c test.h)
  target_link_libraries(test_il_${name} IL::libil)

  add_test(test_il_${name} test_il_${name})
  add_dependencies(check test_il_${name})
  add_dependencies(check_il test_il_${name})
  add_dependencies(check_il_${suite} test_il_${name})
  add_dependencies(test_il_${name} IL::libil)
endmacro ()

il_register_test(source virtual)

il_register_test(codemap emit)
