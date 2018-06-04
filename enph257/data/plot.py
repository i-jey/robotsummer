import matplotlib.pyplot as plt 
import os
from numpy import linspace
import pandas as pd 
from scipy.signal import savgol_filter

# Prompt user for desired file 
dir_path =  os.path.dirname(os.path.realpath(__file__)) 
print(dir_path) 
files = [f for f in os.listdir(dir_path) if os.path.isfile(os.path.join(dir_path, f))]

for i, f in enumerate(files): 
    print(i, f)

user_input = input("Enter the index of the desired file: ")
file_to_plot = files[int(user_input)]

# Read and plot 
df = pd.read_csv(file_to_plot)
time = df["Time(ms)"] / 1000
sensor1 = df["Temp1"]
sensor2 = df["Temp2"]
sensor3 = df["Temp3"]
sensor4 = df["Temp4"]
sensor5 = df["Temp5"]
sensor6 = df["Ambient"]
sensor1_filtered = savgol_filter(sensor1, 51, 9)
sensor2_filtered = savgol_filter(sensor2, 51, 9)
sensor3_filtered = savgol_filter(sensor3, 51, 9)
sensor4_filtered = savgol_filter(sensor4, 51, 9)
sensor5_filtered = savgol_filter(sensor5, 51, 9)
sensor6_filtered = savgol_filter(sensor6, 51, 9)

# plt.style.use('ggplot')
plt.scatter(time, sensor1, alpha=0.90, c='red', label='sensor1')
plt.scatter(time, sensor2, alpha=0.90, c='yellow', label='sensor2')
plt.scatter(time, sensor3, alpha=0.90, c='green', label='sensor3')
plt.scatter(time, sensor4, alpha=0.90, c='blue', label='sensor4')
plt.scatter(time, sensor5, alpha=0.90, c='brown', label='sensor5')
plt.scatter(time, sensor6, alpha=0.90, c='black', label='Ambient')

plt.plot(time, sensor1_filtered, c='red')
plt.plot(time, sensor2_filtered, c='yellow')
plt.plot(time, sensor3_filtered, c='green')
plt.plot(time, sensor4_filtered, c='blue')
plt.plot(time, sensor5_filtered, c='brown')
plt.plot(time, sensor6_filtered, c='black')

plt.xlabel('Time (s)')
plt.ylabel('Temperature (K)')
plt.legend()
plt.show()