default: AGL3-example

run:
	@binary/AGL3-example

%: %.cpp
	@if [ ! -d binary ]; then mkdir binary; fi
	@g++ -I. -pedantic -o binary/$@ *.cpp  -lepoxy -lGL -lglfw 

recompile: clean default

clean:
	@if [ -d binary ]; then rm -rf binary; fi
