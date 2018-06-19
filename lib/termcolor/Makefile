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

.SILENT:
.DEFAULT_GOAL := libtermcolor
.PHONY: clean conf debug dev fclean install re reconf san

BUILD_TYPE ?= Release
BUILD_DIR ?= build/$(BUILD_TYPE)
BUILD_PROJ_DIR = $(BUILD_DIR)/CMakeFiles

CMAKE ?= cmake
CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
CMAKE_G_FLAGS ?= -j8

ifneq (,$(PREFIX))
  CMAKE_FLAGS += -DCMAKE_INSTALL_PREFIX:PATH=$(PREFIX)
endif

ifeq (1,$(VERBOSE))
  CMAKE_FLAGS += -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
  CMAKE_G_FLAGS += VERBOSE=1
endif

ifeq (1,$(TERMCOLOR_TESTING))
  CMAKE_FLAGS += -DTermcolor_TESTING:BOOL=ON
else
  ifeq (0,$(TERMCOLOR_TESTING))
    CMAKE_FLAGS += -DTermcolor_TESTING:BOOL=OFF
  endif
endif

ifeq (1,$(TERMCOLOR_COVERAGE))
  CMAKE_FLAGS += -DTermcolor_COVERAGE:BOOL=ON
else
  ifeq (0,$(TERMCOLOR_COVERAGE))
    CMAKE_FLAGS += -DTermcolor_COVERAGE:BOOL=OFF
  endif
endif

ifeq (1,$(TERMCOLOR_ASAN))
  CMAKE_FLAGS += -DTermcolor_ASAN:BOOL=ON
else
  ifeq (0,$(TERMCOLOR_ASAN))
    CMAKE_FLAGS += -DTermcolor_ASAN:BOOL=OFF
  endif
endif

$(BUILD_DIR) conf:
	mkdir -p $(BUILD_DIR); cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_FLAGS) $(CURDIR)

reconf: mrproper conf

debug dev:
	$(MAKE) BUILD_TYPE=Debug

san:
	$(MAKE) BUILD_TYPE=San

clean:
	[ -d $(BUILD_PROJ_DIR) ] && find $(BUILD_PROJ_DIR) -name "*.o" -delete

fclean:
	[ -d $(BUILD_DIR) ] && find $(BUILD_DIR) -name "*.o" -delete

mrproper:
	$(RM) -rf $(BUILD_DIR)

re: fclean all

install:
	$(CMAKE) --build $(BUILD_DIR) --target install

%: $(BUILD_DIR)
	$(CMAKE) --build $(BUILD_DIR) --target $@ -- $(CMAKE_G_FLAGS)
