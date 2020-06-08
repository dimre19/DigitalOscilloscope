import random
import time
from itertools import count
import matplotlib.pyplot as plt
from matplotlib.axes import Axes
from matplotlib.animation import FuncAnimation
import matplotlib, sys
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
if sys.version_info[0] < 3:
    import Tkinter as Tk
else:
    import tkinter as Tk
import threading

class WindowApp(threading.Thread):

    def __init__(self):
        self.x_values = []
        self.y_values = []
        self.index = count()
        
        threading.Thread.__init__(self)
        self.start()

    def callback(self):
        self.root.quit()
        self.root.destroy()
        print("GUI closed")

    def run(self):
        self.root = Tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW", self.callback)

#        label = tk.Label(self.root, text="Hello World")
#        label.pack()
        self.root.title("Digital Oscilloscope")
        self.root.geometry('800x600')

        plt.style.use('fivethirtyeight')

            


        f = plt.gcf()

        canvas = FigureCanvasTkAgg(f, master=self.root)
        canvas.draw()
        #canvas.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
        canvas.get_tk_widget().place(relx = 0.5, rely = 0.48, relwidth = 1, relheight = 0.8, anchor = 'center' )

        #
        ani = FuncAnimation(f, self.animate, interval = 400)


        #plt.tight_layout()
        #plt.show() #(block = False)
        f.canvas.draw()
        self.root.mainloop()
        
    def animate(self,i):       
        
        #self.add_y_val(random.randint(0, 5))
        #TODO: replace with figure object and scale it's axis    
        plt.cla()
        plt.plot(self.x_values, self.y_values)
        
    def add_y_val(self,y):
        self.x_values.append(next(self.index)*0.5)
        self.y_values.append(y)
            
        if len(self.x_values) > 40:     
            self.x_values.pop(0)
            self.y_values.pop(0)