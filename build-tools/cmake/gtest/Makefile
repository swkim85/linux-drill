all:
	g++ -o main main.c sum.c
test:
	g++ -o sum_test sum_test.c sum.c -isystem /usr/local/include -L/usr/local/lib -pthread -lgtest
	./sum_test
