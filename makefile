
all: 
	cd src; make -f makefile.linux

parallel:
	cd src; make -f makefile.omp

mac:
	cd src; make -f makefile.mac

mpi:
	cd src; make -f makefile.mpi

clean:
	cd src; make -f makefile.linux clean
	rm -f NZVM
