
parser:lex yacc

lex:
	$(LEX) -o../src/parser/sml.yy.cpp sml.yy.lpp
	$(LEX) -Pdata -o../src/parser/data.yy.cpp data.yy.lpp
	
yacc:
	$(YACC) -d -o../src/parser/sml.tab.cpp sml.tab.ypp
	mv ../src/parser/sml.tab.hpp ../src/parser/sml.tab.h
	$(YACC) -pdata -d -o../src/parser/data.tab.cpp data.tab.ypp
	mv ../src/parser/data.tab.hpp ../src/parser/data.tab.h

clean:
	rm -rf ../src/parser/sml.tab.cpp
	rm -rf ../src/parser/sml.tab.h
	rm -rf ../src/parser/sml.yy.cpp
	rm -rf ../src/parser/data.tab.cpp
	rm -rf ../src/parser/data.tab.h
	rm -rf ../src/parser/data.yy.cpp


