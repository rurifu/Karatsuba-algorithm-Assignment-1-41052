"""
Reads results/benchmark-Release.csv and produces:
  1. Log-log runtime vs digit count: schoolbook vs Karatsuba (best cutoff).
  2. Crossover-point plot: how the schoolbook/Karatsuba crossover shifts with cutoff.
  3. Log-log regression to estimate the empirical exponent for each algorithm.
"""
import sys
import csv
import math
from collections import defaultdict
 
import matplotlib.pyplot as plt
import numpy as np
 
path = sys.argv[1] if len(sys.argv) > 1 else "results/benchmark-Release-trialsPerSize=5.csv"
 
# rows[(algorithm, cutoff, digits)] -> list of time_ns
rows = defaultdict(list)
with open(path) as f:
    reader = csv.DictReader(f)
    for r in reader:
        key = (r["algorithm"], r["cutoff"], int(r["digits"]))
        rows[key].append(int(r["time_ns"]))
 
def mean_times(algorithm, cutoff):
    digits_sorted = sorted({d for (a, c, d) in rows if a == algorithm and c == cutoff})
    means = [np.mean(rows[(algorithm, cutoff, d)]) for d in digits_sorted]
    return digits_sorted, means
 
# --- Plot 1: schoolbook vs Karatsuba (pick one representative cutoff, e.g. "16") ---
fig, ax = plt.subplots()
d_school, t_school = mean_times("schoolbook", "NA")
ax.plot(d_school, t_school, marker="o", label="Schoolbook")
 
for cutoff in ["1", "4", "8", "16", "32", "64"]:
    d_k, t_k = mean_times("karatsuba", cutoff)
    if d_k:
        ax.plot(d_k, t_k, marker="o", label=f"Karatsuba (cutoff={cutoff})")
 
ax.set_xscale("log")
ax.set_yscale("log")
ax.set_xlabel("Digits")
ax.set_ylabel("Time (ns)")
ax.set_title("Schoolbook vs Karatsuba: runtime vs input size")
ax.legend()
fig.tight_layout()
fig.savefig("results/runtime_comparison.png", dpi=150)
print("Wrote results/runtime_comparison.png")
 
# --- Plot 2: empirical exponent via log-log regression ---
# min_digits filters out small inputs before fitting. At small n, measured
# time is dominated by fixed overhead (timer resolution, allocation, function
# call cost) rather than the O(n^k) multiplication cost itself, which biases
# the fitted slope away from the true asymptotic exponent. Restricting the
# fit to large n isolates the regime where the algorithm's actual scaling
# behavior dominates the measurement, which allows the graph to resemble
#something closer to the thoeretical number (schoolBook around 2.0, and Karatsuba around 1.585)

def fit_exponent(digits, times, min_digits=0):
    pairs = [(d, t) for d, t in zip(digits, times) if d >= min_digits]
    if len(pairs) < 2:
        return None
    logd = np.log([d for d, t in pairs])
    logt = np.log([t for d, t in pairs])
    slope, intercept = np.polyfit(logd, logt, 1)
    return slope

LARGE_N_THRESHOLD = 8192

print("\nEmpirical exponents (time ~ n^k):")
print(f"{'':30s}{'all sizes':>14s}{'digits >= ' + str(LARGE_N_THRESHOLD):>18s}{'theory':>10s}")

k_all = fit_exponent(d_school, t_school)
k_tail = fit_exponent(d_school, t_school, min_digits=LARGE_N_THRESHOLD)
print(f"{'Schoolbook':30s}{k_all:14.3f}{k_tail:18.3f}{2.0:10.3f}")

for cutoff in ["1", "4", "8", "16", "32", "64"]:
    d_k, t_k = mean_times("karatsuba", cutoff)
    k_all = fit_exponent(d_k, t_k)
    k_tail = fit_exponent(d_k, t_k, min_digits=LARGE_N_THRESHOLD)
    if k_all is not None:
        label = f"Karatsuba (cutoff={cutoff})"
        tail_str = f"{k_tail:18.3f}" if k_tail is not None else f"{'n/a':>18s}"
        print(f"{label:30s}{k_all:14.3f}{tail_str}{1.585:10.3f}")

print(f"\nNote: the 'all sizes' column includes small inputs where fixed overhead")
print(f"(timer resolution, allocation, function-call cost) dominates the actual")
print(f"O(n^k) cost, biasing the fitted slope away from theory. The 'digits >=")
print(f"{LARGE_N_THRESHOLD}' column restricts the fit to the large-n tail, where the")
print(f"asymptotic behavior actually dominates")