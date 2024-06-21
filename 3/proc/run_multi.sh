#!/bin/bash
for (( i = 0; i < 10; i++ ))
do
	./bakery1/bakery_client 127.0.0.1 &
done

