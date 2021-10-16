STANDARD = -std=c++14 

WARNSF = -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie 
WARNS =  -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlarger-than=16384 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wall -Wextra -Wpedantic  -Wno-error=unused-function 

ENABLE_WERROR = -Werror
ENABLE_PIE = -pie
ENABLE_GDB = -g

NUM_ERRORS = -fmax-errors=1

SOURCES = main.cpp stackFunctions.cpp 

DIR_BUILD = build
DIR_RELEASE = $(DIR_BUILD)/release
DIR_DEBUG   = $(DIR_BUILD)/debug


OBJ_FILE_PATH_RELEASE = $(DIR_RELEASE)/stack.out
OBJECTS_RELEASE       = -o $(OBJ_FILE_PATH_RELEASE)

OBJ_FILE_PATH_DEBUG   = $(DIR_DEBUG)/stack.out
OBJECTS_DEBUG         = -o $(OBJ_FILE_PATH_DEBUG) 

COMPILER = g++

all: release

release:
	mkdir -p $(DIR_RELEASE)
	$(COMPILER) $(STANDARD) $(WARNS) 						  				 			    $(SOURCES) $(OBJECTS_RELEASE) 	

debug:
	mkdir -p $(DIR_DEBUG)
	$(COMPILER) $(ENABLE_GDB) $(ENABLE_WERROR) $(STANDARD) $(WARNS) $(WARNSF) $(ENABLE_PIE) $(SOURCES) $(OBJECTS_DEBUG)

clean:
	rm -rf $(DIR_BUILD)/*

run: 
	./$(OBJ_FILE_PATH_RELEASE)

rundbg:
	./$(OBJ_FILE_PATH_DEBUG)

