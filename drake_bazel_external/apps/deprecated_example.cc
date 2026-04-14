// SPDX-License-Identifier: MIT-0

// This example exists to verify that Drake's compile-time deprecation flags
// work correctly for downstream consumers.  It intentionally calls deprecated
// Drake APIs covering the kinds of constructs that DRAKE_DEPRECATED can
// annotate: functions, type aliases, and enum values.
//
// Build with the default behavior (deprecation warnings):
//   bazel build //apps:deprecated_example
//
// Build silencing all deprecation warnings:
//   bazel build //apps:deprecated_example \
//     --@drake//tools/flags:compile_ignore_deprecated=True
//
// Build treating deprecations as hard errors:
//   bazel build //apps:deprecated_example \
//     --@drake//tools/flags:compile_deprecation_is_error=True

#include <filesystem>

// Deprecated free function.
#include "drake/common/file_source.h"
// Deprecated type alias (drake::logging::sink).
#include "drake/common/text_logging_spdlog.h"
// Deprecated enum values (DiscreteContactSolver::kTamsi,
//                         DiscreteContactApproximation::kTamsi).
#include "drake/multibody/plant/multibody_plant.h"

int main() {
  // Deprecated function: drake::to_string(FileSource).
  drake::FileSource source{std::filesystem::path("/tmp/foo.txt")};
  (void)drake::to_string(source);

  // Deprecated type alias: drake::logging::sink.
  drake::logging::sink* sink_ptr = nullptr;
  (void)sink_ptr;

  // Deprecated enum values.
  auto solver = drake::multibody::DiscreteContactSolver::kTamsi;
  (void)solver;
  auto approx = drake::multibody::DiscreteContactApproximation::kTamsi;
  (void)approx;

  return 0;
}
