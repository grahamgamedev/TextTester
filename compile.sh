path='/home/mint/Desktop/Vitasdk/TextTester/'
ip=192.168.43.90

cd $path
cmake . && make && curl -T *.vpk ftp://$ip:1337/ux0:/
