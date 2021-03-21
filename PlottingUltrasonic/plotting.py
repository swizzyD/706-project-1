from matplotlib import pyplot as plt
import numpy as np

times = []
lines = []
distances = []
times2 = []
distances2 = []
filenames = ['150mm', '200mmToWall']


with open(filenames[0] + '.txt','r') as f:
    for line in f:
        line = line.strip()
        x = line.split(", ")
        if len(x[0]) > 0: # has empty list value sometimes
            times.append(int(x[0].split(": ")[1]))
            distances.append(int(x[1].split(" mm")[0]))

fig, (ax1, ax2) = plt.subplots(1, 2)
ax1.plot(times, distances)
ax1.title.set_text('Measured Distance against time for ' + filenames[0] + ' actual distance from barrel tip')
ax1.set_ylabel('Measured Distance (mm)')
ax1.set_xlabel('time (ms)')

# plt.plot(times,distances)
# plt.xlabel('time (ms)')
# plt.ylabel('Measured Distance (mm)')
# plt.title('Measured Distance against time for ' + filenames[0] + ' actual distance from barrel tip')
# plt.show()



with open(filenames[1] + '.txt','r') as f:
    for line in f:
        line = line.strip()
        x = line.split(", ")
        if len(x[0]) > 0: # has empty list value sometimes
            times2.append(int(x[0].split(": ")[1]))
            distances2.append(int(x[1].split(" mm")[0]))

# plt.plot(times,distances)
# plt.xlabel('time (ms)')
# plt.ylabel('Measured Distance (mm)')
# plt.title('Measured Distance against time while sliding sensor from ~200mm to the wall')
# plt.show()


ax2.title.set_text('Measured Distance against time while sliding sensor from ~200mm to the wall')
ax2.set_ylim([0, 200])
ax2.plot(times2,distances2)
ax2.set_ylabel('Measured Distance (mm)')
ax2.set_xlabel('time (ms)')
plt.show()