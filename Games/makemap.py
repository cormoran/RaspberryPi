map_W=128
map_H=128

print 'please input file name you want to create in current directory'
fname=raw_input();
f=open("/Users/Kenya/Documents/programing/RaspberryPi/Games/"+fname,"w")
for i in range(map_H):
    f.write('0'*map_W+'\n')
f.close();
