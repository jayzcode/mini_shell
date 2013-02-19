build: msh

msh: msh.c
	cc $^ -o $@

clean:
	rm -f msh
