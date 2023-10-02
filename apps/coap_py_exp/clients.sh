#!/bin/bash

# Number of clients you want to run (adjust as needed)
num_clients=3

# Loop through clients
for i in $(seq 1 $num_clients); do
  script_name="client_c${i}.py"

  echo "Running $script_name..."
  python "$script_name"

  # Check the exit status of the script
  if [ $? -eq 0 ]; then
    echo "$script_name completed successfully"
  else
    echo "$script_name encountered an error"
  fi
done

# Exit the script
exit 0
