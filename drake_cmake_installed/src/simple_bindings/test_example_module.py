from example_module import SimpleAdder

import numpy as np

from pydrake.systems.analysis import Simulator
from pydrake.systems.framework import (
    DiagramBuilder,
    )
from pydrake.systems.primitives import (
    ConstantVectorSource,
    SignalLogger,
    )

builder = DiagramBuilder()
source = builder.AddSystem(ConstantVectorSource([10.]))
adder = builder.AddSystem(SimpleAdder(100.))
builder.Connect(source.get_output_port(0), adder.get_input_port(0))
logger = builder.AddSystem(SignalLogger(1))
builder.Connect(adder.get_output_port(0), logger.get_input_port(0))
diagram = builder.Build()

simulator = Simulator(diagram)
simulator.StepTo(1)

x = logger.data()
print("Output values: {}".format(x))
assert np.allclose(x, 110.)
