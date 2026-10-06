#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

EXECUTABLES = ["nn", "nn-ikj", "nn-omp"]
LABELS = ["Baseline", "I-K-J ordering", "OpenMP"]

def total_time(df: pd.DataFrame) -> pd.Series:
    num = df.select_dtypes(include="number")
    return num.sum(axis=1)


def exe_path(exe: str) -> str:
    return f"./results/{exe}.csv"


means, stds, labels = [], [], []
for exe, label in zip(EXECUTABLES, LABELS):
    df = pd.read_csv(exe_path(exe))
    totals = total_time(df.dropna(how="all"))
    labels.append(label)
    means.append(totals.mean())
    stds.append(totals.std())

fig, ax = plt.subplots(figsize=(10, 6))
x = range(len(labels))
bars = ax.bar(
    x,
    means,
    yerr=stds,
    capsize=5,
    color="steelblue",
    edgecolor="navy",
    error_kw={"elinewidth": 1.2, "ecolor": "navy"},
)

for xi, m, s in zip(x, means, stds):
    ax.annotate(
        f"{m:.5f}\n± {s:.5f}",
        (xi, m),
        textcoords="offset points",
        xytext=(-30, 8),
        ha="center",
        fontsize=8,
    )

ax.set_xticks(list(x))
ax.set_xticklabels(labels, ha="center")
ax.set_ylabel("Total inference time (milliseconds)")
ax.set_title("Mean total inference time per run (±1 std dev)")
ax.grid(axis="y", linestyle=":", alpha=0.5)
fig.tight_layout()
fig.savefig("results/timing.png", dpi=200)
