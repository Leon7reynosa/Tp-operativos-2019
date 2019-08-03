cd ..
cd ..
cp -r Pool\ Memory/ /home/utnso/
cd configuraciones/
cd prueba_kernel/
cd memoria-1-config/
cp pool.config /home/utnso/Pool\ Memory/
cd ..
cd ..
cd ..
cd ..
mv Pool\ Memory memoria-1-base
cd memoria-1-base/
cd Default/
make clean
make all
