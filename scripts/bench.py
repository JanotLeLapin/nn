#!/usr/bin/env python

import subprocess
import pathlib

EXECUTABLES = ["nn", "nn-ikj", "nn-omp"]
IMAGES = [f"images/{i}.jpg" for i in range(0, 10)]

pathlib.Path("results").mkdir(exist_ok=True)
for exe in EXECUTABLES:
    subprocess.run([f"./{exe}", f"results/{exe}.csv"] + IMAGES)
