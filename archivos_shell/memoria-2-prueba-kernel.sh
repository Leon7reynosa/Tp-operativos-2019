cd ..
cp -r Pool\ Memory/ /home/utnso/
cd configuraciones/
cd prueba_kernel/
cd memoria-2-config/
cp pool.config /home/utnso/Pool\ Memory/
cd ..
cd ..
cd ..
cd ..
mv Pool\ Memory memoria-2-prueba-kernel
cd memoria-2-prueba-kernel/
cd Default/
make clean
make all
