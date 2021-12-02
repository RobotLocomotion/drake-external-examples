# Copyright (c) 2018, Toyota Research Institute.
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
# * Neither the name of the copyright holder nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

"""
Provides an example of using pybind11-bound Drake C++ system with pydrake.
"""

from __future__ import print_function

from simple_adder import SimpleAdder, SimpleAdder_

import numpy as np

from pydrake.autodiffutils import AutoDiffXd
from pydrake.symbolic import Expression
from pydrake.systems.analysis import Simulator
from pydrake.systems.framework import DiagramBuilder
from pydrake.systems.primitives import (
    ConstantVectorSource,
    VectorLogSink,
)


def main():
    # Simulate with doubles.
    builder = DiagramBuilder()
    source = builder.AddSystem(ConstantVectorSource([10.]))
    adder = builder.AddSystem(SimpleAdder(100.))
    builder.Connect(source.get_output_port(0), adder.get_input_port(0))
    logger = builder.AddSystem(VectorLogSink(1))
    builder.Connect(adder.get_output_port(0), logger.get_input_port(0))
    diagram = builder.Build()

    simulator = Simulator(diagram)
    simulator.AdvanceTo(1)

    x = logger.FindLog(simulator.get_context()).data()
    print("Output values: {}".format(x))
    assert np.allclose(x, 110.)

    # Compute outputs with AutoDiff and Symbolic.
    for T in (AutoDiffXd, Expression):
        adder_T = SimpleAdder_[T](100.)
        context = adder_T.CreateDefaultContext()
        adder_T.get_input_port().FixValue(context, [10.])
        output = adder_T.AllocateOutput()
        adder_T.CalcOutput(context, output)
        # N.B. At present, you cannot get a reference to existing AutoDiffXd
        # or Expression numpy arrays, so we will explictly copy the vector:
        # https://github.com/RobotLocomotion/drake/issues/8116
        value, = output.get_vector_data(0).CopyToVector()
        assert isinstance(value, T)
        print("Output from {}: {}".format(type(adder_T), repr(value)))


if __name__ == "__main__":
    main()
