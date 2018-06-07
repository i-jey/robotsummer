import matplotlib.pyplot as plt 
import os
from numpy import linspace
import pandas as pd 
from matplotlib.widgets import CheckButtons
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
fig, ax = plt.subplots()

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

plt.style.use('ggplot')
s1, = ax.plot(time, sensor1, alpha=0.30, c='red', label='sensor1')
s2, = ax.plot(time, sensor2, alpha=0.30, c='yellow', label='sensor2')
s3, = ax.plot(time, sensor3, alpha=0.30, c='green', label='sensor3')
s4, = ax.plot(time, sensor4, alpha=0.30, c='blue', label='sensor4')
s5, = ax.plot(time, sensor5, alpha=0.30, c='brown', label='sensor5')
s6, = ax.plot(time, sensor6, alpha=0.30, c='black', label='Ambient')

s1f, = ax.plot(time, sensor1_filtered, alpha=0.75, c='red')
s2f, = ax.plot(time, sensor2_filtered, alpha=0.75, c='yellow')
s3f, = ax.plot(time, sensor3_filtered, alpha=0.75, c='green')
s4f, = ax.plot(time, sensor4_filtered, alpha=0.75, c='blue')
s5f, = ax.plot(time, sensor5_filtered, alpha=0.75, c='brown')
s6f, = ax.plot(time, sensor6_filtered, alpha=0.75, c='black')
plt.xlabel('Time (s)')
plt.ylabel('Temperature (C)')
plt.legend()

rax = plt.axes([0, 0.2, 0.1, 0.15])
check = CheckButtons(rax, ('1', '2', '3', '4', '5', '6'), (True, True, True, True, True, True))

def func(label): 
    if label == '1': 
        s1.set_visible(not s1.get_visible())
        s1f.set_visible(not s1f.get_visible())
    elif label == '2': 
        s2.set_visible(not s2.get_visible())
        s2f.set_visible(not s2f.get_visible())
    elif label == '3': 
        s3.set_visible(not s3.get_visible())
        s3f.set_visible(not s3f.get_visible())
    elif label == '4': 
        s4.set_visible(not s4.get_visible())
        s4f.set_visible(not s4f.get_visible())
    elif label == '5': 
        s5.set_visible(not s5.get_visible())
        s5f.set_visible(not s5f.get_visible())
    elif label == '6': 
        s6.set_visible(not s6.get_visible())
        s6f.set_visible(not s6f.get_visible())
    plt.draw()
check.on_clicked(func) 


plt.show()


# bin_size = 3.0
# num bins = (start time - end time) / bin_size
# bins_array = np.linspace(start time, endtime, bin)

# bin_indexs = np.digitize(data,bins)
# data[i] belongs to bin (bin_indexes[i])

# for q in range(0,max(bins array))
#     data_to_median = data[np.where(bin_indexs == q)
#     one_media = np.median(data to median)
#     big median array append (one_ median)