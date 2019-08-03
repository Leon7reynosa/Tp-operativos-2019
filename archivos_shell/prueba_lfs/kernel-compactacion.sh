cd ..
cd ..
cp -r Kernel/ /home/utnso/
cd configuraciones/
cd prueba_kernel/
cd kernel-config/
cp kernel.config /home/utnso/Kernel/
cd ..
cd ..
cd ..
cd ..
mv Kernel kernel-compactacion
cd kernel-compactacion/
cd Default/
make clean
make all
