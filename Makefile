all:
	cc -Wall -o ppos-test ppos-core-aux.c pingpong-contab-prio.c libppos_static.a
	@echo "Compilation successful!"
