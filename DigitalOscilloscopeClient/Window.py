#import tkinter

#root = tkinter.Tk()
#
#root.title("DigitalOscilloscope")
#root.geometry('400x300')

#root.mainloop()

#import matplotlib, numpy, sys
#matplotlib.use('TkAgg')
#from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
#from matplotlib.figure import Figure
#if sys.version_info[0] < 3:
#    import Tkinter as Tk
#else:
#    import tkinter as Tk
#
#root = Tk.Tk()
#
#f = Figure(figsize=(5,4), dpi=100)
#ax = f.add_subplot(111)
#
#data = (20, 35, 30, 35, 27)
#
#ind = numpy.arange(5)  # the x locations for the groups
#width = .5
#
#rects1 = ax.bar(ind, data, width)
#
#canvas = FigureCanvasTkAgg(f, master=root)
#canvas.show()
#canvas.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
#
#Tk.mainloop()


#import matplotlib.pyplot as plt
#
#plt.axis([0, 10, 0, 1])
#
#for i in range(10):
#    y = i
#    plt.scatter(i, y)
#    plt.pause(1)
#
#plt.show()

import random
from itertools import count
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

plt.style.use('fivethirtyeight')

x_values = []
y_values = []

index = count()


def animate(i):
    if i < 10:
        x_values.append(next(index))
        y_values.append(random.randint(0, 5))
        
    else:        
        y_values[i % 10] = (random.randint(0, 5))
        
        
    plt.cla()
    plt.plot(x_values, y_values)


ani = FuncAnimation(plt.gcf(), animate, 1000)


plt.tight_layout()
plt.show()