# SPDX-License-Identifier: MIT-0

"""
Our CMakeLists.txt file disabled the CSDP solver as part of the Drake build.
Here, we'll check that the opt-out succeeded.
"""

import unittest

from pydrake.solvers import CsdpSolver

class TestCsdpSolver(unittest.TestCase):
    def test_unavailable(self):
        solver = CsdpSolver()
        self.assertFalse(solver.available())

if __name__ == '__main__':
    unittest.main()
