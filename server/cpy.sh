arm-linux-gnueabihf-gcc  ./source/*.c -lsqlite3 -o server
cp ./server /home/linux/rootfs/
cp ./Database /home/linux/rootfs/ -arf