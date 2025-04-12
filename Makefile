


all:
	g++ -O3 -I src -std=c++11 src/*.cpp -o hcal


install:
	sudo cp hcal /usr/local/bin



