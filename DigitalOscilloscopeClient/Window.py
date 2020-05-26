
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
import matplotlib, sys
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
if sys.version_info[0] < 3:
    import Tkinter as Tk
else:
    import tkinter as Tk


#f = plt.Figure(figsize = (9,5), dpi = 100)
def on_closing(): #Question dialog box at exit
    root.quit()     # stops mainloop
    root.destroy()
    #if messagebox.askokcancel("Quit", "Do you want to quit?"):
        
        
def animate(i):
    
    x_values.append(next(index))
    y_values.append(random.randint(0, 5))
        
    if i > 10:     
        x_values.pop(0)
        y_values.pop(0)
        
        
    plt.cla()
    plt.plot(x_values, y_values)
#    f.cla()
#    f.plot(x_values, y_values)

#ani = FuncAnimation(f, animate, interval = 1000)
root = Tk.Tk()
plt.style.use('fivethirtyeight')

x_values = []
y_values = []

index = count()
f = plt.gcf()

canvas = FigureCanvasTkAgg(f, master=root)
canvas.draw()
#canvas.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
canvas.get_tk_widget().place(relx = 0.5, rely = 0.48, relwidth = 1, relheight = 0.8, anchor = 'center' )

#
ani = FuncAnimation(f, animate, interval = 1000)


#plt.tight_layout()
#plt.show() #(block = False)
f.canvas.draw()
root.protocol("WM_DELETE_WINDOW", on_closing)
root.mainloop() #Tk.mainloop also works. any difference?
#TODO: How to exit program?
#plt.close()
print('Done')