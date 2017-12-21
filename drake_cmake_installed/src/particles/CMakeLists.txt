# -*- mode: cmake -*-
# vi: set ft=cmake :

# Copyright (c) 2017, Massachusetts Institute of Technology.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the Massachusetts Institute of Technology nor the names
#   of its contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE MASSACHUSETTS INSTITUTE OF TECHNOLOGY AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
# NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE MASSACHUSETTS
# INSTITUTE OF TECHNOLOGY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# send over the sdf file so the particle example can be run 'in place'.
configure_file(models/particle.sdf ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_library(particles particle.cc utilities.cc)
target_link_libraries(particles drake::drake)

find_package(gflags CONFIG REQUIRED)

add_executable(uniformly_accelerated_particle_demo uniformly_accelerated_particle.cc)
target_link_libraries(uniformly_accelerated_particle_demo particles
  drake::drake drake::drake-common-text-logging-gflags gflags::gflags)

find_package(GTest MODULE)

if(GTEST_FOUND)
  add_executable(test_particle test/test_particle.cc)
  target_link_libraries(test_particle
    GTest::GTest GTest::Main particles drake::drake)
  add_executable(test_utilities test/test_utilities.cc)
  target_link_libraries(test_utilities
    GTest::GTest GTest::Main particles drake::drake)

  add_test(NAME test_particle COMMAND test_particle)
  add_test(NAME test_utilities COMMAND test_utilities)
endif()
