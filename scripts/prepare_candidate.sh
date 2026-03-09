#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "Usage: $0 <candidate-name-slug> [candidate-repo-path]"
    echo "Example candidate-name-slug: john-doe"
    exit 1
fi

candidate_name="$1"
if [[ ! "${candidate_name}" =~ ^[a-z0-9]+(-[a-z0-9]+)*$ ]]; then
    echo "Error: candidate-name-slug must match '[a-z0-9]+(-[a-z0-9]+)*'."
    echo "Use lowercase letters, digits, and hyphens only (for example: john-doe)."
    exit 1
fi

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
internal_repo="$(cd "${script_dir}/.." && pwd)"
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
candidate_repo="${2:-${default_candidate_repo}}"
base_branch="candidate/task-price-fix"
candidate_branch="candidate/${candidate_name}"

if [[ -z "${candidate_repo}" || ! -d "${candidate_repo}/.git" ]]; then
    echo "Error: candidate repo not found at ${candidate_repo}"
    exit 1
fi

cd "${candidate_repo}"

if [[ -n "$(git status --porcelain)" ]]; then
    echo "Error: working tree is not clean. Commit or stash changes first."
    exit 1
fi

if ! git show-ref --verify --quiet "refs/heads/${base_branch}"; then
    echo "Error: base branch '${base_branch}' does not exist."
    exit 1
fi

if ! git check-ref-format --branch "${candidate_branch}" >/dev/null 2>&1; then
    echo "Error: generated branch name '${candidate_branch}' is not a valid git branch name."
    exit 1
fi

if git show-ref --verify --quiet "refs/heads/${candidate_branch}"; then
    echo "Error: candidate branch '${candidate_branch}' already exists."
    exit 1
fi

git checkout "${base_branch}"
git checkout -b "${candidate_branch}"

echo "Created candidate branch: ${candidate_branch}"
echo "Repository: ${candidate_repo}"
