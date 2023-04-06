# SPDX-License-Identifier: MIT-0

set(CTEST_PROJECT_NAME "drake-external-examples")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")
set(CTEST_DROP_METHOD "https")
set(CTEST_DROP_SITE "drake-cdash.csail.mit.edu")
set(CTEST_DROP_LOCATION "/submit.php?project=${CTEST_PROJECT_NAME}")
set(CTEST_DROP_SITE_CDASH ON)
