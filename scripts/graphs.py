import numpy as np
import matplotlib.pyplot as plt

threads8_parallel = ["barnesHutParallelN8_2_10.txt",
                      "barnesHutParallelN8_2_11.txt",
                      "barnesHutParallelN8_2_12.txt",
                      "barnesHutParallelN8_2_13.txt",
                      "barnesHutParallelN8_2_14.txt",
                      "barnesHutParallelN8_2_15.txt" ]

threads16_parallel = ["barnesHutParallelN16_2_10.txt",
                      "barnesHutParallelN16_2_11.txt",
                      "barnesHutParallelN16_2_12.txt",
                      "barnesHutParallelN16_2_13.txt",
                      "barnesHutParallelN16_2_14.txt",
                      "barnesHutParallelN16_2_15.txt"]

threads32_parallel = ["barnesHutParallelN32_2_10.txt",
                      "barnesHutParallelN32_2_11.txt",
                      "barnesHutParallelN32_2_12.txt",
                      "barnesHutParallelN32_2_13.txt",
                      "barnesHutParallelN32_2_14.txt",
                      "barnesHutParallelN32_2_15.txt"]
#                      "barnesHutParallelN32_2_20.txt"]                                                                                                     

threads64_parallel = ["barnesHutParallelN64_2_10.txt",
                      "barnesHutParallelN64_2_11.txt",
                      "barnesHutParallelN64_2_12.txt",
                      "barnesHutParallelN64_2_13.txt",
                      "barnesHutParallelN64_2_14.txt",
                      "barnesHutParallelN64_2_15.txt"]
#                      "barnesHutParallelN64_2_20.txt"]                                                                                                     

threads68_parallel = ["barnesHutParallelN68_2_10.txt",
                      "barnesHutParallelN68_2_11.txt",
                      "barnesHutParallelN68_2_12.txt",
                      "barnesHutParallelN68_2_13.txt",
                      "barnesHutParallelN68_2_14.txt",
                      "barnesHutParallelN68_2_15.txt"]
#                      "barnesHutParallelN68_2_20.txt"]                                                                                                     

x_labels = ["2^10", "2^11", "2^12", "2^13", "2^14", "2^15", "2^20"]
x_values = [2**10 , 2**11 , 2**12 , 2**13 , 2**14, 2**15, 2**20]

def getYValues(input_list):
    y_values = []
    for fname in input_list:
        f = open(fname)
        time_str = f.readline()
        print(fname, time_str)
        time_us = int(time_str)
        y_values.append(time_us)
        f.close()

    return y_values


threads8_time_us  = getYValues(threads8_parallel)
threads16_time_us = getYValues(threads16_parallel)
threads32_time_us = getYValues(threads32_parallel)
threads64_time_us = getYValues(threads64_parallel)
threads68_time_us = getYValues(threads68_parallel)


plt.plot(x_values[0:len(threads8_time_us)], threads8_time_us, c="red", label="8 Threads")
plt.scatter(x_values[0:len(threads8_time_us)], threads8_time_us, c="red")

plt.plot(x_values[0:len(threads16_time_us)], threads16_time_us, c="blue", label="16 Threads")
plt.scatter(x_values[0:len(threads16_time_us)], threads16_time_us, c="blue")

plt.plot(x_values[0:len(threads32_time_us)], threads32_time_us, c="green", label="32 Threads")
plt.scatter(x_values[0:len(threads32_time_us)], threads32_time_us, c="green")

plt.plot(x_values[0:len(threads64_time_us)], threads64_time_us, c="purple", label="64 Threads")
plt.scatter(x_values[0:len(threads64_time_us)], threads64_time_us, c="purple")

plt.plot(x_values[0:len(threads68_time_us)], threads68_time_us, c="orange", label="68 Threads")
plt.scatter(x_values[0:len(threads68_time_us)], threads68_time_us, c="orange")

plt.xlabel("Number of Bodies")
plt.ylabel("Time (us)")
plt.title("Barnes-Hut Simulation Time vs Number of Bodes")
plt.legend()
plt.show()
