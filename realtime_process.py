import serial
import time

#startup
filename = '/home/pi/arduino2pi/data/' + time.strftime("%Y%m%d") + '_v2.csv'
open_file = open(filename, "a")
print(open_file)

def output(s):
    global open_file
    global filename
    T = time.strftime("%Y,%m,%d,%H,%M,%S,")
    #print(T + s)
    open_file.write(T + s + '\n')
    nfile = '/home/pi/arduino2pi/data/' + time.strftime("%Y%m%d") + '_v2.csv'
    if nfile != filename:
        open_file.close()
        open_file = open(nfile, "a")
        filename = nfile

def run():
    ser = serial.Serial('/dev/ttyUSB0',9600)
    PIR_LIST, MIC_V_LIST, MIC_C_LIST, PHR_S_LIST, BMP_T_LIST, BMP_P_LIST = [], [], [], [], [], []
    workingwith = time.strftime("%S")
    while True:
        read_serial = ser.readline()
        s = ser.readline().decode().strip()
        E = s.split(',')
        if len(E) < 6:
            continue
        _, PIR, MIC_V, MIC_C, PHR_S, BMP_T, BMP_P = E
        PIR_LIST.append(int(PIR))
        MIC_V_LIST.append(float(MIC_V))
        MIC_C_LIST.append(int(MIC_C))
        PHR_S_LIST.append(int(PHR_S))
        BMP_T_LIST.append(float(BMP_T))
        BMP_P_LIST.append(float(BMP_P))
        cur = time.strftime("%S")
        if cur != workingwith:
            Plen = len(PIR_LIST)
            if Plen > 0:
                Pmin = min(PIR_LIST)
                Pmax = max(PIR_LIST)
                Psum = sum(PIR_LIST)
                MVmin = min(MIC_V_LIST)
                MVmax = max(MIC_V_LIST)
                MVsum = sum(MIC_V_LIST)
                #MCmin = min(MIC_C_LIST)
                #MCmax = max(MIC_C_LIST)
                #MCsum = sum(MIC_C_LIST)
                PHRmin = min(PHR_S_LIST)
                PHRmax = max(PHR_S_LIST)
                BTmin = min(BMP_T_LIST)
                BTmax = max(BMP_T_LIST)
                BTsum = sum(BMP_T_LIST)
                BPmin = min(BMP_P_LIST)
                BPmax = max(BMP_P_LIST)
                BPsum = sum(BMP_P_LIST)
                #PHRsum = sum(PHR_S_LIST)
                acc = ','.join([str(round(i, 2)) for i in [Plen, Pmin, Pmax, Psum, MVmin, MVmax, MVsum, PHRmin, PHRmax, BTmin, BTmax, BTsum, BPmin, BPmax, BPsum]])
                output(acc)
            PIR_LIST, MIC_V_LIST, MIC_C_LIST, PHR_S_LIST, BMP_T_LIST, BMP_P_LIST = [], [], [], [], [], []
            workingwith = cur


if __name__ == '__main__':
    while True:
        try:
            run()
            time.sleep(5)
        except:
            pass

