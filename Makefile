gcc: src/main.c src/TAD/analise_frequencia.c
	gcc -o EXEC_GCC src/main.c src/TAD/analise_frequencia.c -Wall
run: ./EXEC_GCC
	./EXEC_GCC $(filter-out $@, $(MAKECMDGOALS))
%:
	@true