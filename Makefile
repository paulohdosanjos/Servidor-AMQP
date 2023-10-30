all : broker

broker : broker.c queue.c utils.c state.c hardcoded.c
	@gcc -o broker broker.c queue.c utils.c state.c hardcoded.c

hard: make_hardcoded.c utils.c
	@gcc -o hard make_hardcoded.c utils.c


docker:
	docker run -d --hostname rmq --name rabbit-server -p 8080:15672 -p 5672:5672 rabbitmq:3-management

clean:
	rm broker
