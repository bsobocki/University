default: labyrinth

run:
	@./run

labyrinth: main.cpp
	@if [ ! -d binary ]; then mkdir binary; fi
	@g++ -I. -pedantic -o binary/labyrinth *.cpp  -lepoxy -lGL -lglfw 

recompile: clean default

clean:
	@if [ -d binary ]; then rm -rf binary; fi
