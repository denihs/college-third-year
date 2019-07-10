from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
ext_modules = [
    Extension("cliente",  ["codigo/cliente.pyx"]),
    Extension("servidor",  ["codigo/servidor.pyx"]),
		Extension("comunicacao", ["codigo/comunicacao.pyx"]),
]
setup(
    name = 'cliente-servidor',
    cmdclass = {'build_ext': build_ext},
    ext_modules = ext_modules
)
