all:
	bin/pip install -Ur requirements.txt; \
	. bin/activate; \
	python3 setup.py build_ext --inplace

clean:
	rm -rf *.so codigo/*.c build
