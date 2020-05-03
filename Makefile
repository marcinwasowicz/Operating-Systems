Histogram:
	gcc Histogram.c -o hist -lpthread
Image:
	python imageMaker.py $(name) $(width) $(height)
clean:
	rm hist
Experiment:
	make Image name=$(name) width=$(width) height=$(height)
	./hist $(threads) $(method) $(name) out