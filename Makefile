CXX = g++

CXX_FLAGS = -I include -I lib/Verror/include -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations 	\
 -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported  	\
 -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security 				\
 -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual 	\
 -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn	\
 -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand 															\
 -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast 			\
 -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow 																	\
 -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla 															\
 -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

SRCS = src/parser/parser.cpp src/parser/first_step.cpp src/parser/dump.cpp src/parser/main.cpp src/parser/tree.cpp
OBJ = $(patsubst %.cpp, build/%.o, $(subst src/parser/, , $(SRCS))) lib/Verror/lib/libVerror.a
EXECUTABLE = language
VALGRIND = valgrind --leak-check=full --leak-resolution=med ./$(EXECUTABLE)

all: $(OBJ)
	@echo "CXX $(EXECUTABLE)"
	@$(CXX) $(CXX_FLAGS) -lasan $(OBJ) -o $(EXECUTABLE)
	
build/%.o: src/parser/%.cpp
	mkdir -p ./build
	@$(CXX) $(CXX_FLAGS) -c -o $@ $<

lib/Verror/lib/libVerror.a:
	make clean -C lib/Verror/
	make lib -C lib/Verror/
.PHONY: clean mem

clean:
	@rm -f build/*.o
	@rm -f $(EXECUTABLE)

mem:
	valgrind --leak-check=full --leak-resolution=med ./$(EXECUTABLE)
	
