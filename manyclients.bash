#!/bin/bash

for N in {1..50}
do
	ruby client.rb $(($N))
done
wait
