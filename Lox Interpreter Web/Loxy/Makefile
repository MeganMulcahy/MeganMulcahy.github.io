.PHONY: all run clean

all: build run

build:
	@echo "Building Lox interpreter..."
	dotnet build -o bin

run:
	@echo "Running Lox interpreter..."
	dotnet run --project CraftingInterpreters.Lox.csproj tester.lox 2>&1 | grep -v 'make: \*\*\* \[run\]'

clean:
	@echo "Cleaning up..."
	rm -rf bin
	dotnet clean
