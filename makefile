
all: 
	cd src; make -f makefile.linux


clean:
	cd src; make -f makefile.linux clean
	rm -f NZVM
