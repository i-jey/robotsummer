import serial 
import keyboard 
import time 
import datetime

def collect_data(): 
    start_time = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d_%H-%M-%S')
    file_name = str(start_time) + '.txt'
    file = open("data/" + file_name, "w")
    file.write("Time(ms),Temp1,Temp2,Temp3,Temp4,Temp5")
    
    ser = serial.Serial('COM8', 9600, timeout=1)
    serial.Serial.reset_input_buffer(ser)
    print("Continuously reading from serial port...") 
    print("Press 'a' to end.")

    while (True): 
        line = str(ser.readline().rstrip())
        line = line[2:]
        line = line[:-2]
        file.write(str(line))
        file.write('\n')

        if keyboard.is_pressed('a'):
            return
    return 

collect_data()