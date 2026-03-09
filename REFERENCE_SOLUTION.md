# Reference Solution

## Expected minimum
- add VIP test
- identify VIP bug
- fix VIP discount from 25% to 20%

## Stronger variant
- add negative-price handling
- validate NaN/infinity input
- refactor discount selection into helper function
- improve tests for edge cases

## Included reference C++ files (changed files only)

See:
- `reference/include/PriceCalculator.h`
- `reference/src/PriceCalculator.cpp`
- `reference/tests/PriceCalculatorTests.cpp`

## Verify reference solution

Run:

```bash
./scripts/verify_reference_solution.sh
```

These files are for interviewers only and are not meant to be exposed to candidates.
