macd: src/main.c src/configuration.c
	clang -framework ApplicationServices -lcJSON -o macd src/main.c src/configuration.c
