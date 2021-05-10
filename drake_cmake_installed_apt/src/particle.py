# Copyright (c) 2021, Massachusetts Institute of Technology.
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

from pydrake.systems.framework import BasicVector
from pydrake.systems.framework import LeafSystem
from pydrake.systems.framework import PortDataType


class Particle(LeafSystem):
    """
    A linear 1DOF particle system.

    With very simple dynamics xdotdot = a, this system can be described in
    terms of its:

    Inputs:
        linear acceleration (input index 0), in m/s^2 units.

    States/Outputs:
        linear position (state/output index 0), in m units.
        linear velocity (state/output index 1), in m/s units.
    """
    def __init__(self):
        LeafSystem.__init__(self)
        # A 1D input vector for acceleration.
        self.DeclareInputPort('acceleration', PortDataType.kVectorValued, 1)
        # Adding one generalized position and one generalized velocity.
        self.DeclareContinuousState(1, 1, 0)
        # A 2D output vector for position and velocity.
        self.DeclareVectorOutputPort('postion_and_velocity', BasicVector(2),
                                     self.CopyStateOut)

    def CopyStateOut(self, context, output):
        # Get current state from context.
        continuous_state_vector = context.get_continuous_state_vector()
        # Write system output.
        output.SetFromVector(continuous_state_vector.CopyToVector())

    def DoCalcTimeDerivatives(self, context, derivatives):
        # Get current state from context.
        continuous_state_vector = x = context.get_continuous_state_vector()
        # Obtain the structure we need to write into.
        derivatives_vector = derivatives.get_mutable_vector()
        # Get current input acceleration value.
        input_vector = self.EvalVectorInput(context, 0)
        # Set the derivatives. The first one is velocity and the second one is
        # acceleration.
        derivatives_vector.SetAtIndex(0, continuous_state_vector.GetAtIndex(1))
        derivatives_vector.SetAtIndex(1, input_vector.GetAtIndex(0))
