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
mv Kernel kernel-base
cd kernel-prueba-base/
cd Default/
make clean
make all
