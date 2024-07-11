all:
	gcc  ./source/*.c -lsqlite3 -o server
arm:
	arm-linux-gnueabihf-gcc  ./source/*.c -lsqlite3 -o server
	cp ./server /home/linux/rootfs/
	cp ./Database /home/linux/rootfs/ -arf
go:
	./server 127.0.0.1 8888
clean:
	rm -f server