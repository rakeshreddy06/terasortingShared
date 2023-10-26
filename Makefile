CC=gcc
CFLAGS=-Wall
OFLAGS=-O3

# Define data sizes in bytes: 1GB, 4GB, 16GB, 64GB
SIZES := 10000000 40000000 160000000 640000000

# Data file names
FILES := $(foreach s,$(SIZES),data_$(s).txt)

all: mysort $(FILES)

mysort: mysort.c
	$(CC) $(CFLAGS) $(OFLAGS) -o mysort mysort.c

data_%.txt:
	./gensort -a $* $@

run: mysort $(FILES)
	$(foreach file,$(FILES),./mysort $(file);)

clean:
	rm -f mysort $(FILES)
