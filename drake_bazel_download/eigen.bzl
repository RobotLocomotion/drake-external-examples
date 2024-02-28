def _impl(repo_ctx):
    repo_ctx.symlink("/usr/include/eigen3", "include")
    repo_ctx.symlink(Label("@//:eigen.BUILD.bazel"), "BUILD.bazel")

local_eigen_repository = repository_rule(
    implementation = _impl,
)
