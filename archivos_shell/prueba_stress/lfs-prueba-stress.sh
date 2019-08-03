cd ..
cd ..
cp -r LFS/ /home/utnso/
cd configuraciones/
cd prueba_stress/
cd lfs-config/
cp fileSystem.config /home/utnso/LFS/
cd ..
cd metadata-config/
cp Metadata.bin /home/utnso/LFS/Metadata/
cd ..
cd ..
cd ..
cd ..
mv LFS lfs-stress
cd lfs-stress/
cd Default/
make clean
make all

