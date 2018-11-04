DIST=./dist

CACTUS_IN=./imgs/cactus.png
CACTUS_OUT=$(DIST)/cactus.txt

DINO_IN=./imgs/dino.png
DINO_OUT=$(DIST)/dino.txt

MAIN_PY=./index.py

all: build
build:
	python3 $(MAIN_PY) $(CACTUS_IN) > $(CACTUS_OUT)
	python3 $(MAIN_PY) $(DINO_IN) > $(DINO_OUT)