# SPDX-License-Identifier: MIT-0

"""
Provides an example of disabling the open-source CSDP solver.
"""

import unittest

from pydrake.solvers import CsdpSolver

class TestCsdpSolver(unittest.TestCase):
    def test_unavailable(self):
        solver = CsdpSolver()
        self.assertFalse(solver.available())

if __name__ == '__main__':
    unittest.main()
