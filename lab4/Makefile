.PHONY:
	all build test clean \
	build_a run_a build_b run_b

all: run_a run_b

run_a:
	@echo "run program A\n"
	@cd lab4a && $(MAKE) all

run_b:
	@echo "run program B\n"
	@cd lab4b && $(MAKE) all
