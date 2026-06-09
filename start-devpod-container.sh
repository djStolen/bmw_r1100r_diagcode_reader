#!/bin/bash

LOG_FILE="/tmp/devcontainer_build_$(pwd | md5sum | awk '{print $1}').log"

# 0. Delete prior Devpod Container
echo "[+] Deleting prior DevPod container..."
devpod delete .

# 1. Start the DevPod environment in the background, redirecting stdout to a build log
echo "[+] Spinning up DevPod in the background"
devpod up . > "$LOG_FILE" 2>&1 &
DEV_PID=$!

# Wait briefly for DevPod to initialize writing to the log file
sleep 2

# 2. Actively trace/stream the live Docker compilation output to the screen
echo "[+] Showing live Docker build trace (Press Ctrl+C to stop watching logs, container will keep building)..."
echo "--------------------------------------------------------------------------------"
tail -f "$LOG_FILE" &
TAIL_PID=$!

# Trap Ctrl+C so if you exit the log stream, it kills the 'tail' process but leaves the Docker build running
trap "kill $TAIL_PID 2>/dev/null; echo -e '\n[Dropped live log streaming]'; exit" INT

# Wait for the background DevPod engine process to finish completely
wait $DEV_PID
kill $TAIL_PID 2>/dev/null

echo "--------------------------------------------------------------------------------"
echo "[+] Docker build sequence finished."
echo ""

# 3. Dynamically grab the workspace name for this directory
WORKSPACE_NAME=$(devpod list | grep "$(pwd)" | awk '{print $1}')

if [ -z "$WORKSPACE_NAME" ]; then
    # Fallback if grep path matches weirdly, fetch the absolute last line's workspace name
    WORKSPACE_NAME=$(devpod list | tail -n 1 | awk '{print $1}')
fi

echo "[+] Target Workspace detected: $WORKSPACE_NAME"
echo "------------------------------------------------"

# 4. Check the definitive final status of the container
echo "[+] Current Workspace Status:"
devpod list | grep "$WORKSPACE_NAME"

echo "------------------------------------------------"

# 5. Info on how to ssh into the container
echo "[+] If you want to ssh into the container run:"
echo "devpod ssh $WORKSPACE_NAME"

# Clean up the temporary build log file
rm -f "$LOG_FILE"

