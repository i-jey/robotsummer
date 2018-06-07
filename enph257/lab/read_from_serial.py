
'''
NOTE: DEPRECATED 2018-06-03 
We are now logging data directly from Arduino to an SD card.  
'''

import serial 
import keyboard 
import time 
import datetime

def collect_data(): 
    start_time = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d_%H-%M-%S')
    file_name = str(start_time) + '.txt'
    file = open("data/" + file_name, "w")
    file.write("Time(ms),Temp1,Temp2,Temp3,Temp4,Temp5,Ambient")
    
    ser = serial.Serial('COM8', 9600, timeout=1)
    serial.Serial.reset_input_buffer(ser)
    ser.flush()
    print("Continuously reading from serial port...") 
    print("Press 'a' to end.")

    while (True): 
        line = str(ser.readline().rstrip())
        line = line[2:]
        line = line[:-2]
        if line: 
            file.write(str(line))
            file.write('\n')
            print(line)

        if keyboard.is_pressed('a'):
            return
    return 

collect_data()