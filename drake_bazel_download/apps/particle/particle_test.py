# SPDX-License-Identifier: MIT-0

import unittest

from particle import Particle


class TestParticle(unittest.TestCase):
    """A test case for Particle systems."""
    def setUp(self):
        # System (aka 'device under test') being tested.
        self.dut = Particle()
        # Context for the given dut.
        self.context = self.dut.CreateDefaultContext()
        # Outputs of the given dut.
        self.output = self.dut.AllocateOutput()
        # Derivatives of the given dut.
        self.derivatives = self.dut.AllocateTimeDerivatives()

    def test_output(self):
        """
        Makes sure a Particle output is consistent with its state (position and
        velocity).
        """
        # Initialize state.
        continuous_state_vector = \
            self.context.get_mutable_continuous_state_vector()
        continuous_state_vector.SetAtIndex(0, 10.0)  # x0 = 10 m.
        continuous_state_vector.SetAtIndex(1, 1.0)  # x1 = 1 m/s.
        # Compute outputs.
        self.dut.CalcOutput(self.context, self.output)
        output_vector = self.output.get_vector_data(0)
        # Check results.
        self.assertEqual(output_vector.GetAtIndex(0), 10.0)  # y0 == x0
        self.assertEqual(output_vector.GetAtIndex(1), 1.0)  # y1 == x1

    def test_derivatives(self):
        """
        Makes sure a Particle system state derivatives are consistent with its
        state and input (velocity and acceleration).
        """
        # Set input.
        input_port = self.dut.get_input_port(0)
        input_port.FixValue(self.context, 1.0)  # u0 = 1 m/s^2
        # Set state.
        continuous_state_vector = \
            self.context.get_mutable_continuous_state_vector()
        continuous_state_vector.SetAtIndex(0, 0.0)  # x0 = 0 m
        continuous_state_vector.SetAtIndex(1, 2.0)  # x1 = 2 m/s
        # Compute derivatives.
        self.dut.CalcTimeDerivatives(self.context, self.derivatives)
        derivatives_vector = self.derivatives.get_vector()
        # Check results.
        self.assertEqual(derivatives_vector.GetAtIndex(0), 2.0)  # x0dot == x1
        self.assertEqual(derivatives_vector.GetAtIndex(1), 1.0)  # x1dot == u0


if __name__ == '__main__':
    unittest.main()
