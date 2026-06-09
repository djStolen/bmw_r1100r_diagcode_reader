#!/bin/bash

# 0. Delete prior Devpod Container
devpod delete .

# 1. Start the DevPod environment completely detached in the background
echo "[+] Spinning up DevPod in the background..."
devpod up . > /dev/null 2>&1 & disown

# Wait briefly for DevPod to initialize the workspace tracking
sleep 2

echo "------------------------------------------------"

# 2. Dynamically grab the workspace name for this directory
WORKSPACE_NAME=$(devpod list | grep "$(pwd)" | awk '{print $1}')

if [ -z "$WORKSPACE_NAME" ]; then
    # Fallback if grep path matches weirdly, fetch the absolute last line's workspace name
    WORKSPACE_NAME=$(devpod list | tail -n 1 | awk '{print $1}')
fi

echo "[+] Target Workspace detected: $WORKSPACE_NAME"
echo "------------------------------------------------"

# 3. Output the current real-time DevPod build logs
echo "[+] Showing initial DevPod build logs (Press Ctrl+C to exit logs streaming)..."
devpod logs "$WORKSPACE_NAME"

echo "------------------------------------------------"

# 4. Check the definitive final status of the container
echo "[+] Current Workspace Status:"
devpod list | grep "$WORKSPACE_NAME"

echo "------------------------------------------------"

# 5. Info on how to ssh into the container
echo "[+] If you want to ssh into the container run:"
echo "devpod ssh $WORKSPACE_NAME"
