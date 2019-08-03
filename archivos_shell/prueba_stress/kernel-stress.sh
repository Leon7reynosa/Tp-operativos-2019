cd ..
cd ..
cp -r Kernel/ /home/utnso/
cd configuraciones/
cd prueba_stress/
cd kernel-config/
cp kernel.config /home/utnso/Kernel/
cd ..
cd ..
cd ..
cd ..
mv Kernel kernel-stress
cd kernel-prueba-stress/
cd Default/
make clean
make all
