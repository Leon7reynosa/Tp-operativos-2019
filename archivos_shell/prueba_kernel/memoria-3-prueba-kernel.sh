cd ..
cd ..
cp -r Pool\ Memory/ /home/utnso/
cd configuraciones/
cd prueba_kernel/
cd memoria-3-config/
cp pool.config /home/utnso/Pool\ Memory/
cd ..
cd ..
cd ..
cd ..
mv Pool\ Memory memoria-3-prueba-kernel
cd memoria-3-prueba-kernel/
cd Default/
make clean
make all
