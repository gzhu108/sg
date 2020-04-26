cd ..\..
mkdir build
docker run -it -v%cd%:/sg -w/sg/build --rm gzhu108/sgbuilder cmake ..
docker run -it -v%cd%:/sg -w/sg/build --rm gzhu108/sgbuilder make -j 10