#!/bin/bash

# Number of clients you want to run (adjust as needed)
num_clients=10

# Loop through clients
for i in $(seq 0 $((num_clients-1))); do
  script_name="clients_all.py"

  echo "Running $script_name... with arg ${i} "
  python "$script_name" -cid $i

  # Check the exit status of the script
  if [ $? -eq 0 ]; then
    echo "$script_name completed successfully"
  else
    echo "$script_name encountered an error"
  fi
done

# Exit the script
exit 0
