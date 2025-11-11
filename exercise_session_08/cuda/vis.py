import pandas as pd
import matplotlib.pyplot as plt
import re
import numpy as np

filename = "results.txt"

blocks = []
threads = []
times = []

with open(filename, "r") as f:
    for line in f:
        m = re.search(r"NUM_BLOCK=(\d+) NUM_THREAD=(\d+) -> .* computed in ([\d\.eE+-]+) seconds", line)
        if m:
            blocks.append(int(m.group(1)))
            threads.append(int(m.group(2)))
            times.append(float(m.group(3)))

df = pd.DataFrame({"Block": blocks, "Thread": threads, "Time": times})

heatmap = df.pivot(index="Thread", columns="Block", values="Time")

plt.figure(figsize=(10,6))
c = plt.imshow(heatmap, origin="lower", aspect="auto", cmap="viridis")
plt.colorbar(c, label="Time (s)")
plt.xticks(np.arange(len(heatmap.columns)), heatmap.columns)
plt.yticks(np.arange(len(heatmap.index)), heatmap.index)
plt.xlabel("Number of Blocks")
plt.ylabel("Threads per Block")
plt.title("CUDA PI Computation Time")
plt.savefig("cuda_pi_runtime.png", dpi=300, bbox_inches="tight")
plt.show()
