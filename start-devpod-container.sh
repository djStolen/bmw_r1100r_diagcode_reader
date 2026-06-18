#!/bin/bash

LOG_FILE="/tmp/devcontainer_build_$(pwd | md5sum | awk '{print $1}').log"

# 0. Delete prior Devpod Container
echo "[+] Deleting prior DevPod container..."
devpod delete .

# 1. Start the DevPod environment (using default browser IDE)
echo "[+] Spinning up DevPod with Browser IDE..."
devpod up . > "$LOG_FILE" 2>&1 &
DEV_PID=$!

# Wait briefly for DevPod to initialize writing to the log file
sleep 2

# 2. Actively trace/stream the live Docker compilation output to the screen
echo "[+] Showing live Docker build trace..."
echo "--------------------------------------------------------------------------------"

# Run a background log tailer
tail -f "$LOG_FILE" &
TAIL_PID=$!

# Monitor the log file: break out as soon as the browser link is active
while kill -0 $DEV_PID 2>/dev/null; do
    if grep -qE "Successfully started|done Successfully|Starting vscode in browser mode" "$LOG_FILE"; then
        echo -e "\n[+] Browser IDE is ready!"
        break
    fi
    sleep 1
done

# Stop streaming logs to the screen
kill $TAIL_PID 2>/dev/null
echo "--------------------------------------------------------------------------------"
echo "[+] Docker build sequence finished."
echo ""

# 3. Dynamically grab the workspace name for this directory
WORKSPACE_NAME=$(devpod list | grep "$(pwd)" | awk '{print $1}')

if [ -z "$WORKSPACE_NAME" ]; then
    WORKSPACE_NAME=$(devpod list | tail -n 1 | awk '{print $1}')
fi

echo "[+] Target Workspace detected: $WORKSPACE_NAME"
echo "------------------------------------------------"

# 4. Check the definitive final status of the container
echo "[+] Current Workspace Status:"
devpod list | grep "$WORKSPACE_NAME"

echo "------------------------------------------------"

# 5. Output access links directly from the log file
echo "[+] IDE Access Information:"
grep -E "Starting vscode in browser mode|Successfully opened" "$LOG_FILE" | sed 's/.*info //g' || echo "Access at http://localhost:10800"

echo ""
echo "[+] If you want to ssh into the container instead, run:"
echo "devpod ssh $WORKSPACE_NAME"
echo "------------------------------------------------"

# Clean up the temporary log
rm -f "$LOG_FILE"
