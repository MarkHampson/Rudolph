import numpy as np
import matplotlib

numpoints = 400
x = np.linspace(.0,np.log(2.0),numpoints) #points spaced between 1 and log(2.0)

lg = np.floor(np.expm1(x) * 8192)
lg = lg.astype(int) # copy as an int array

print(lg[1:-1])

f = open('pulseWidthTable.h', 'w')
f.write('static const int pulseWidth[] = { \n')

for idx,i in enumerate(lg[1:-2]):
    f.write(str(lg[idx]) + ', \n')
f.write(str(lg[-1]) + '\n')
f.write('};\n')

f.close()
