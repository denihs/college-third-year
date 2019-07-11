all: venv
	. bin/activate; \
	python3 setup.py build_ext --inplace

venv: bin/activate
bin/activate:
	test -d venv || virtualenv venv
	bin/pip install -Ur requirements.txt
	touch bin/activate

clean:
	rm -rf *.so codigo/*.c build
