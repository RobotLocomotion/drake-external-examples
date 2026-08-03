# SPDX-License-Identifier: MIT-0

"""Test the Drake find resource API."""

import logging

from pydrake.common import FindResourceOrThrow

# If you have trouble finding resources, you can enable debug logging to see
# how `FindResource*` is searching.
logging.getLogger("drake").setLevel(logging.DEBUG)

FindResourceOrThrow("drake/examples/pendulum/Pendulum.urdf")
