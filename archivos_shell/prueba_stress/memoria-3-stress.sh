cd ..
cd ..
cp -r Pool\ Memory/ /home/utnso/
cd configuraciones/
cd prueba_stress/
cd memoria-3-config/
cp pool.config /home/utnso/Pool\ Memory/
cd ..
cd ..
cd ..
cd ..
mv Pool\ Memory memoria-3-stress
cd memoria-3-stress/
cd Default/
make clean
make all
