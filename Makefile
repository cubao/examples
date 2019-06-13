.PHONY: all reset_submodules clean

all:
	@echo nothing special

reset_submodules:
	git submodule update --init --recursive

clean:
	rm -rf build dist
