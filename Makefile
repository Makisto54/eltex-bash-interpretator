.PHONY: clean

all: bash

bash: bash.c
	gcc bash.c -o bash

clean:
	rm -rf build main