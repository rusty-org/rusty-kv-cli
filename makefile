.PHONY: build-cli
build-cli:
	@echo "Building CLI..." && \
		mkdir -p cli/tmp && \
		g++ -Wall -o cli/tmp/main \
			cli/main.cpp \
			cli/client/client.cpp \
			cli/utils/connect.cpp \
			-std=c++17 \
			-licuuc -licuio && \
		echo "CLI built successfully, binary -> ./cli/tmp/main"

.PHONY: run-cli
run-cli: build-cli
	./cli/tmp/main

.PHONY: run-server
run-server:
	@cargo run

.PHONY: run
run:
	@$(MAKE) -j2 run-cli run-server
