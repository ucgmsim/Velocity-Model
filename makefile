
all: 
	cd src; make -f makefile.linux


clean:
	cd src; make clean
	rm -f NZVM
