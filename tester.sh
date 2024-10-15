#!/usr/bin/env bash

# Define color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

mapfile -t executables < <(find /bin/ -type f -executable)

for executable in "${executables[@]}"; do
  nm_result=$(nm "$executable" 2>&1)

  ft_nm_result=$(./ft_nm "$executable" 2>&1)

  # Remove the prefix and compare the results
  nm_result_clean=$(echo "$nm_result" | sed '/^nm/d')
  ft_nm_result_clean=$(echo "$ft_nm_result" | sed '/^ft_nm/d')

  if diff <(echo "$nm_result_clean") <(echo "$ft_nm_result_clean") > /dev/null; then
    echo -e "${YELLOW}$executable${NC} => ${GREEN}OK${NC}"
  else
    echo -e "${YELLOW}$executable${NC} => ${RED}KO${NC}"
  fi
done