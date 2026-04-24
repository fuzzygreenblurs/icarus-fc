#!/usr/bin/env bash
set -euo pipefail

cppcheck --addon=misra --std=c11 --enable=style --error-exitcode=1 src/

