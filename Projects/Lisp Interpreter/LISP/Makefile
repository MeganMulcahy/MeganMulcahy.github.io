.PHONY: all run clean

all: build run

build:
	@echo "Building Lisp interpreter..."
	dotnet build -o bin

run:
	@echo "Running Lisp interpreter..."
	dotnet run --project Lsp.csproj tester.lsp

clean:
	@echo "Cleaning up..."
	rm -rf bin
	dotnet clean
