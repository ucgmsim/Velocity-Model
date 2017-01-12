
all: 
	cd src; make -f makefile.linux

parallel:
	cd src; make -f makefile.omp

clean:
	cd src; make -f makefile.linux clean
	rm -f NZVM
