cd ..
cd ..
cp -r Kernel/ /home/utnso/
cd configuraciones/
cd prueba_memoria/
cd kernel-config/
cp kernel.config /home/utnso/Kernel/
cd ..
cd ..
cd ..
cd ..
mv Kernel kernel-prueba-memoria
cd kernel-prueba-memoria/
cd Default/
make clean
make all
