CC = gcc

main = main

matches = matches.txt

players = players.txt

output = output.html

windows: 
	$(CC) -fdiagnostics-color=always -g $(main).c -o $(main) -lm
	./$(main).exe $(matches) $(players) $(output)

linux: 
	$(CC) -fdiagnostics-color=always -g $(main).c -o $(main) -lm
	./$(main) $(matches) $(players) $(output)
	xdg-open $(output)