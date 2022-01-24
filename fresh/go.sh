rm -rf bios.txt
./klogd 22 -a $1 -i eth1 -s 10
cat bios.txt |sort | uniq > mfu.txt
oopsnr2=`grep -c . mfu.txt`
echo "  "
echo "Found $oopsnr2 ip's "
echo -e "                \033[1;33m\033[1;32m   FRESH \033[1;33m\033[0m"
sleep 10
export PATH=.:PATH
hald
rm -rf bios.txt
