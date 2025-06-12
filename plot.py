import os
import numpy as np
import matplotlib.pyplot as plt

linkrate = [0.01, 0.06, 0.11, 0.16, 0.21, 0.26, 0.31, 0.36, 0.41, 0.46, 0.51, 0.53, 0.55, 0.57, 0.59, 0.61, 0.63, 0.65, 0.67, 0.69, 0.71, 0.73, 0.75, 0.77, 0.79, 0.81, 0.83, 0.85]
latency = [27, 27, 28, 28, 28, 28, 28, 29, 29, 29, 30, 30, 29, 30, 30, 30, 32, 31, 31, 31, 31, 31, 34, 33, 34, 37, 37, 51]
traffic = [0.01, 0.0599, 0.1098, 0.1596,0.2093,0.2592,0.3091,0.3589,0.4086,0.4586,0.5085,0.5283,0.5481,0.5684,0.586501,0.6084,0.6269,0.6477,0.6672,0.6875,0.7065,0.7276,0.7435,0.7657,0.7829,0.7951,0.8177,0.7673]
plt.plot(linkrate, latency)
plt.ylabel("Average packet delay (cycles)")
plt.xlabel("Packet injection rate [packet/cycle/node]")
plt.savefig("pic/latency.png")
plt.show()

plt.plot(linkrate, traffic)
plt.ylabel("Throughput [flits/cycle/node]")
plt.xlabel("Packet injection rate [packet/cycle/node]")
plt.savefig("pic/throughput.png")
plt.show()