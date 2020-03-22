programs:
	gcc binaryDecoder.c -o bindecod
	gcc Matrix.c -o Matrix
	gcc ProcessTree.c -o tree
	gcc PasteExecutor.c -o pasteexec
test:
	./mmult $(a) $(b) $(n)
	./displayFromSingleFile $(a) $(b) $(c)
cleanPrograms:
	rm bindecod
	rm Matrix
	rm tree
	rm pasteexec
cleanMatrixes:
	rm matrix*
copy:
	cp -r /home/mkwasowicz/sysopy/lab3 /mnt/c/Users/marci/Desktop/Sysopy