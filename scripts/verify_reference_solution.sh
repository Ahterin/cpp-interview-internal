#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
internal_repo="$(cd "${script_dir}/.." && pwd)"
reference_dir="${internal_repo}/reference"
base_ref="candidate/task-price-fix"
original_tag="interview/original-candidate-task-price-fix"
default_candidate_repo=""
search_dir="${internal_repo}"
for _ in 1 2 3 4 5; do
    if [[ -d "${search_dir}/cpp-interview-exercise/.git" ]]; then
        default_candidate_repo="${search_dir}/cpp-interview-exercise"
        break
    fi
    if [[ -d "${search_dir}/.git" && -f "${search_dir}/CMakeLists.txt" ]]; then
        default_candidate_repo="${search_dir}"
        break
    fi
    search_dir="$(cd "${search_dir}/.." && pwd)"
done
candidate_repo="${1:-${default_candidate_repo}}"

if [[ -z "${candidate_repo}" || ! -d "${candidate_repo}/.git" ]]; then
    echo "Error: candidate repo not found at ${candidate_repo}"
    exit 1
fi

reset_ref="${base_ref}"
if git -C "${candidate_repo}" show-ref --verify --quiet "refs/tags/${original_tag}"; then
    reset_ref="${original_tag}"
fi

if ! git -C "${candidate_repo}" rev-parse --verify "${reset_ref}^{commit}" >/dev/null 2>&1; then
    echo "Error: reset reference '${reset_ref}' was not found in candidate repo."
    exit 1
fi

temp_dir="$(mktemp -d)"
trap 'rm -rf "${temp_dir}"' EXIT

work_repo="${temp_dir}/cpp-interview-exercise"
mkdir -p "${work_repo}"
cp -a "${candidate_repo}/." "${work_repo}/"
git -C "${work_repo}" reset --hard "${reset_ref}" >/dev/null
git -C "${work_repo}" clean -fdx >/dev/null

cp "${reference_dir}/include/PriceCalculator.h" "${work_repo}/include/PriceCalculator.h"
cp "${reference_dir}/src/PriceCalculator.cpp" "${work_repo}/src/PriceCalculator.cpp"
cp "${reference_dir}/tests/PriceCalculatorTests.cpp" "${work_repo}/tests/PriceCalculatorTests.cpp"

cmake -S "${work_repo}" -B "${work_repo}/build"
cmake --build "${work_repo}/build"
ctest --test-dir "${work_repo}/build" --output-on-failure --verbose

echo "Baseline reference: ${reset_ref}"
echo "Reference solution verification passed."
