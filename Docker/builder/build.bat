cd ..\..
docker run -it -v%cd%:/sg -w/sg --rm gzhu108/sgbuilder cmake CMakelists.txt
docker run -it -v%cd%:/sg -w/sg --rm gzhu108/sgbuilder make -j 10