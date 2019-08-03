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
mv Kernel kernel-prueba-kernel
cd kernel-prueba-kernel/
cd Default/
make clean
make all
