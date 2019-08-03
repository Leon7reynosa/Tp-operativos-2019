cd ..
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
mv Pool\ Memory memoria-2-compactacion
cd memoria-2-compactacion/
cd Default/
make clean
make all
