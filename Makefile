firstbin := main
secondbin := client
firstobj := main.o
secondobj := client.o

all: $(firstbin) $(secondbin)

$(firstbin): $(firstobj) 
	gcc $< -o $(firstbin) -lrt

$(secondbin): $(secondobj) 
	gcc $< -o $(secondbin) -lrt 

%.o: %.c
	gcc -c $< -o $@ 

clean:
	rm $(firstbin) $(firstobj) $(secondbin) $(secondobj)

